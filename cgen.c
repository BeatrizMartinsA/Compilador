#include "globals.h"
#include "symtab.h"
#include "code.h"
#include "cgen.h"
#define MAX_ARGS 4

static int temp = 0;

static int label = 0;

static int buscar_irmao = 1;

static int label_func_actual = 0;

static int cGen (Node * no);

static int genDcl (Node * no) {
   int ret, op;
   int base, pos;
   int aux;
   int cont, param;
   int inicio_while, fim_while;
   char * name;
   Node * brother;
   switch (no->tipo.dcl) {
      case tif:
         ret = cGen(no->filho[0]); //condição contrária do if, resultado tem que guardar em algum lugar
         
         fprintf(code, "(IF, $t%d, L%d , -)\n", ret, label); // se condição ao contrário é verdadeira, pula pro else

         cGen(no->filho[1]); // entra no bloco if
         // preciso ir pro fim do bloco if-else, já que nem sempre o fim da condição é um return pra ir pro fim da função
         fprintf(code, "(GOTO, L%d, -, -)\n", label+1);

         fprintf(code, "(LABEL, L%d, - , -)\n", label); // bloco else
         label++;
         cGen(no->filho[2]);

         // label do fim do bloco if-else
         fprintf(code, "(LABEL, L%d, - , -)\n", label);
         label++;
      break;

      case twhile:
         inicio_while = label;
         fprintf(code, "(LABEL, L%d, - , -)\n", inicio_while); // label começo do while
         label++;
         fim_while = label;
         label++;

         op = cGen(no->filho[0]); // checa condição oposta
         fprintf(code, "(IF, $t%d, L%d , -)\n", op, fim_while); // fim do while

         ret = cGen(no->filho[1]); // executa a parte de dentro do while
         fprintf(code, "(GOTO, L%d, -, -)\n", inicio_while); // volta para o inicio para checar a condição de novo

         fprintf(code, "(LABEL, L%d, - , -)\n", fim_while); // label do fim do while
      break;

      case tigual:
         if (no->filho[0]->filho[0] != NULL)
            pos = cGen(no->filho[0]->filho[0]);
         else 
            pos = -1;

         base = cGen(no->filho[1]);
         if (pos == -1) 
            fprintf(code, "(STORE, $t%d, %s , -)\n", base, no->filho[0]->atr.nome);
         else
            fprintf(code, "(STORE, $t%d, %s , $t%d)\n", base, no->filho[0]->atr.nome, pos);
         temp = 0;
      break;

      case tvariavel:
         fprintf(code, "(ALLOC, %s, %s, -)\n", no->atr.nome, no->atr.escopo);
      break;
      
      case tvector:
         fprintf(code, "(ALLOC_V, %s, %s, %d)\n", no->atr.nome, no->atr.escopo, no->atr.len);
         return -1;
      break;
      
      case tfunction:
         switch (no->typeExp) {
            case tint:
               fprintf(code, "(FUNC, int, %s, -)\n", no->atr.nome);
            break;
            
            case tvoid:
               fprintf(code, "(FUNC, void, %s, -)\n", no->atr.nome);
            break;

            case tbool:
               fprintf(code, "(FUNC, bool, %s, -)\n", no->atr.nome);
            break;

            default:
            break;
         }
         cGen(no->filho[0]); // args

         //label de começo da função
         fprintf(code, "(LABEL, L%d, - , -)\n", label);
         label++;
         label_func_actual = label;
         label++;

         cGen(no->filho[1]); // interior função

         //fim da função
         fprintf(code, "(LABEL, L%d, -, -)\n", label_func_actual);
         fprintf(code, "(END, %s, - , -)\n", no->atr.nome);

      break;
      
      case tparam:
         aux = no->tipo.exp;
         switch (no->typeExp) {
            case tint:
               fprintf(code, "(ARG, int, %s, %s)\n", no->atr.nome, no->atr.escopo);
            break;
            
            case tvoid:
               fprintf(code, "(ARG, void, %s, %s)\n", no->atr.nome, no->atr.escopo);
            break;

            case tbool:
               fprintf(code, "(ARG, bool, %s, %s)\n", no->atr.nome, no->atr.escopo);
            break;

            default:
            break;
         }
      break;

      case tcall:
         buscar_irmao = 0;
         cont = 0;
         name = no->atr.nome;
         if (no->filho[0] != NULL) {
            param = cGen(no->filho[0]);
            fprintf(code, "(PARAMETRO, $t%d, -, -)\n", param);
            cont++;
            brother = no->filho[0]->irmao;

            while (brother != NULL) {
               param = cGen(brother);
               fprintf(code, "(PARAMETRO, $t%d, -, -)\n", param);
               cont++;
               brother = brother->irmao;
            }
         }
         fprintf(code, "(CALL, $t%d, %s, %d)\n", temp, name, cont);
         buscar_irmao = 1;
         return temp++;
      break;
      
      case treturn:
         ret = cGen(no->filho[0]);
         fprintf(code, "(RETURN, $t%d, -, -)\n", ret);
         fprintf(code, "(GOTO, L%d, -, -)\n", label_func_actual);
      break;

      default:
      break;
   }
}

static int genExp (Node *no) {
   int aux1, aux2, aux3;
   int assign;
   int operando1, operando2;
   switch (no->tipo.exp) {
      case op:
         operando1 = cGen(no->filho[0]);
         operando2 = cGen(no->filho[1]);

         switch (no->atr.op) {
            case SOMA:
               fprintf(code, "(SOMA, $t%d, $t%d, $t%d)\n", operando1, operando2, temp);
            break;

            case SUB:
               fprintf(code, "(SUB, $t%d, $t%d, $t%d)\n", operando1, operando2, temp);
            break;

            case MULT:
               fprintf(code, "(MULT, $t%d, $t%d, $t%d)\n", operando1, operando2, temp);
            break;

            case DIV:
               fprintf(code, "(DIV, $t%d, $t%d, $t%d)\n", operando1, operando2, temp);
            break;

            case EHIGUAL:
               // como é if false, pula quando é diferente
               fprintf(code, "(DIFERENTE, $t%d, $t%d, $t%d)\n", operando1, operando2, temp);
            break;

            case DIFERENTE:
               // como é if false, pula quando é igual
               fprintf(code, "(IGUAL, $t%d, $t%d, $t%d)\n", operando1, operando2, temp);
            break;

            case MENOR:
               // como é if false, pula quando é maior ou igual
               fprintf(code, "(MAIORIGUAL, $t%d, $t%d, $t%d)\n", operando1, operando2, temp);
            break;

            case MAIOR:
               // como é if false, pula quando é menor ou igual
               fprintf(code, "(MENORIGUAL, $t%d, $t%d, $t%d)\n", operando1, operando2, temp);
            break;

            case MENORIGUAL:
               // como é if false, pula quando é maior
               fprintf(code, "(MAIOR, $t%d, $t%d, $t%d)\n", operando1, operando2, temp);
            break;

            case MAIORIGUAL:
               // como é if false, pula quando é menor
               fprintf(code, "(MENOR, $t%d, $t%d, $t%d)\n", operando1, operando2, temp);
            break;

            default:
            break;
         }
         return temp++;
      break;
      
      case constante:
         fprintf(code, "(CONST, $t%d, %d , -)\n", temp, no->atr.val);
         return temp++;
      break;
      
      case id:
         fprintf(code, "(LOAD, $t%d, %s , -)\n", temp, no->atr.nome);
         return temp++;
      break;
      
      case vector:
         aux1 = cGen(no->filho[0]); // temp que vai guardar o indice do vetor

         if (aux1 < 0) { // não passei a posição do vetor que quero acessar
            if (strcmp(eh_parametro(no->atr.nome, no->atr.escopo), "sim") == 0)
               fprintf(code, "(LOAD_VP, $t%d, %s , -)\n", temp, no->atr.nome);
            else
               fprintf(code, "(LOAD_V, $t%d, %s, -)\n", temp, no->atr.nome);
         }
         else {
            if (strcmp(eh_parametro(no->atr.nome, no->atr.escopo), "sim") == 0)
               fprintf(code, "(LOAD_VP, $t%d, %s , $t%d)\n", temp, no->atr.nome, aux1);
            else
               fprintf(code, "(LOAD_V, $t%d, %s, $t%d)\n", temp, no->atr.nome, aux1);
         }
         assign = temp;
         temp++;
         fprintf(code, "(ASSIGN, $t%d, $t%d, -)\n", temp, assign);
         aux2 = temp;
         temp++;
         return aux2;
      break;
      
      case type:
         cGen(no->filho[0]);
      break;

      default:
      break;
   }
}

static int cGen (Node * no) { 
   int auxTemp = -1;
   if (no != NULL) { 
      switch (no->tipoNo) {
         case declaration:
            auxTemp = genDcl(no);
            break;

         case expression:
            auxTemp = genExp(no);
            break;

         default:
            printf("OLÁ");
            break;
      }
    if (no->irmao != NULL && buscar_irmao == 1) 
      auxTemp = cGen(no->irmao);
  }
  return auxTemp;
}

void intermediarioGen(Node * syntaxTree, char * codefile) {  
   char * s = malloc(strlen(codefile)+7);
   strcpy(s, "File: ");
   strcat(s, codefile);

   emitComment("Gerador de código intermediário");
   emitComment(s);

   fprintf(code, "Código intermediário:\n");

   cGen(syntaxTree);

   emitComment("End of execution.");
}