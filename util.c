#include "globals.h"
#include "util.h"

void printToken( TokenType token, const char* tokenString )
{ 
  switch (token)
  { 
    case IF: fprintf(listing, "Palavra Reservada: %s\n", tokenString); break;
    case ELSE: fprintf(listing, "Palavra Reservada: %s\n", tokenString); break;
    case WHILE: fprintf(listing, "Palavra Reservada: %s\n", tokenString); break;
    case INT: fprintf(listing, "Palavra Reservada: %s\n", tokenString); break;
    case VOID: fprintf(listing, "Palavra Reservada: %s\n", tokenString); break;
    case RETURN: fprintf(listing, "Palavra Reservada: %s\n", tokenString); break;
    case SOMA: fprintf(listing, "Simbolo: +\n"); break;
    case SUB: fprintf(listing, "Simbolo: -\n"); break;
    case MULT: fprintf(listing, "Simbolo: *\n"); break;
    case DIV: fprintf(listing, "Simbolo: /\n"); break;
    case IGUAL: fprintf(listing, "Simbolo: =\n"); break;
    case MENOR: fprintf(listing, "Simbolo: <\n"); break;
    case MAIOR: fprintf(listing, "Simbolo: >\n"); break;
    case MENORIGUAL: fprintf(listing, "Simbolo: <=\n"); break;
    case MAIORIGUAL: fprintf(listing, "Simbolo: >=\n"); break;
    case EHIGUAL: fprintf(listing, "Simbolo: ==\n"); break; 
    case DIFERENTE: fprintf(listing, "Simbolo: !=\n"); break;
    case ABREPARENTESE: fprintf(listing, "Simbolo: (\n"); break;
    case FECHAPARENTESE: fprintf(listing, "Simbolo: )\n"); break;
    case ABRECOLCHETE: fprintf(listing, "Simbolo: [\n"); break;
    case FECHACOLCHETE: fprintf(listing, "Simbolo: ]\n"); break;
    case ABRECHAVE: fprintf(listing, "Simbolo: {\n"); break;
    case FECHACHAVE: fprintf(listing, "Simbolo: }\n"); break;
    case VIRGULA: fprintf(listing, "Simbolo: ,\n"); break;
    case PONTOVIRGULA: fprintf(listing, "Simbolo: ;\n"); break;
    case FIM: fprintf(listing,"EOF\n"); break;
    case ID: fprintf(listing,"ID: %s\n",tokenString); break;
    case NUM: fprintf(listing,"Número: %s\n",tokenString); break;
    case ERRO: fprintf(listing,"Erro: %s\n",tokenString); break;
    default: fprintf(listing,"Token Desconhecido: %d\n",token);
  }
}

Node * declarationNode(DeclarationType tipo)
{ 
  Node * t = (Node *) malloc(sizeof(Node));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n", lineno);
  else {
    for (i=0;i<MAXFILHOS;i++) t->filho[i] = NULL;
    t->irmao = NULL;
    t->tipoNo = declaration;
    t->tipo.dcl = tipo;
    t->lineno = lineno;
  }
  return t;
}


Node * expressionNode(ExpressionID tipo)
{ 
  Node * t = (Node *) malloc(sizeof(Node));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXFILHOS;i++) 
      t->filho[i] = NULL;
    t->irmao = NULL;
    t->tipoNo = expression;
    t->tipo.exp = tipo;
    t->lineno = lineno;
    t->typeExp = tvoid;
  }
  return t;
}

char * copyString(char * s)
{ 
  int n;
  char * t;
  if (s==NULL) return NULL;
  n = strlen(s)+1;
  t = malloc(n);
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else strcpy(t,s);
  return t;
}

static int indentno = 0;


#define INDENT indentno+=2
#define UNINDENT indentno-=2


static void printSpaces(void)
{ 
  int i;
  for (i=0;i<indentno;i++)
    fprintf(listing," ");
}


void printTree( Node * tree )
{ 
  int i;
  INDENT;
  while (tree != NULL) {
    printSpaces();
    if (tree->tipoNo == declaration)
    { 
      switch (tree->tipo.dcl) {
        case tif:
          fprintf(listing,"If\n");
          break;
        case tigual:
          fprintf(listing,"Atribuicao\n");
          break;
        case twhile:
          fprintf(listing,"While\n");
          break;
        case tvariavel:
          fprintf(listing,"Variavel %s\n", tree->atr.nome);
          break;
        case tvector:
          fprintf(listing,"Vetor %s\n", tree->atr.nome);
          break;
        case tparam:
          fprintf(listing,"Parametro %s\n", tree->atr.nome);
          break;
        case tfunction:
          fprintf(listing,"Funcao %s\n", tree->atr.nome);
          break;
        case tcall:
          fprintf(listing,"Chamada da funcao %s\n", tree->atr.nome);
          break;        
        case treturn:
          fprintf(listing,"Return\n");
          break;        
              
        default:
            fprintf(listing,"Tipo de nó de declaracao desconhecido\n");
            break;
      }
    }
    else if (tree->tipoNo == expression)
    { 
      switch (tree->tipo.exp) {
        case op:
          fprintf(listing,"Operacao: ");
          printToken(tree->atr.op,"\0");
          break;
        case constante:
          fprintf(listing,"Constante: %d\n",tree->atr.val);
          break;
        case id:
          fprintf(listing,"Id: %s\n",tree->atr.nome);
          break;
	      case vector:
          fprintf(listing,"Vetor: %s\n",tree->atr.nome);
          break;
	      case type:
          fprintf(listing,"Tipo %s\n",tree->atr.nome);
          break;
        default:
          fprintf(listing,"Tipo de nó de expressao desconhecido\n");
          break;
      }
    }
    else fprintf(listing,"Unknown node kind\n");
    for (i=0;i<MAXFILHOS;i++)
      printTree(tree->filho[i]);
    tree = tree->irmao;
  }
  UNINDENT;
}