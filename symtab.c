#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

#define SIZE 211 /* tam hash*/
#define SHIFT 4 /* hash alpha = 2⁴ = 16*/

static int hash (char* nome_id, char* escopo) { 
    int temp = 0, i = 0;
    while (nome_id[i] != '\0') { 
        temp = ((temp << SHIFT) + nome_id[i]) % SIZE;
        i++;
    }
    i = 0;
    while (escopo[i] != '\0') { 
        temp = ((temp << SHIFT) + escopo[i]) % SIZE;
        i++;
    }
    return temp;
}

/* the list of line numbers of the source 
 * code in which a variable is referenced
 */
typedef struct LineListRec {
    int lineno;
    struct LineListRec * next;
} * LineList;

/* The record in the bucket lists for
 * each variable, including name, 
 * assigned memory location, and
 * the list of line numbers in which
 * it appears in the source code
 */
typedef struct BucketListRec { 
    int indice ; /* memory location for variable */
    char * nome_id;
    char *escopo;
    char * tipo_id;
    char * tipo_dado;
    char * param;
    char * vetor;
    LineList lines;
    struct BucketListRec * next;
} * BucketList;

/* the hash table */
static BucketList hashTable[SIZE];

void insere_tabela(int indice, char * nome_id, char *escopo, char * tipo_id, int tipo_dado, int param, int vetor, int lineno) {
    int h;
    h = hash(nome_id, escopo);
    BucketList item =  hashTable[h];
    while ((item != NULL) && ((strcmp(tipo_id, item->tipo_id) != 0) || (strcmp(nome_id, item->nome_id) != 0) || (strcmp(escopo,item->escopo) != 0)))
        item = item->next;
    /* variable not yet in table */
    if (item == NULL) {
        item = (BucketList)malloc(sizeof(struct BucketListRec));
        item->nome_id = nome_id;
        item->escopo = escopo;
        item->tipo_id = tipo_id;
        //item->tipo_dado = tipo_dado;
        if (tipo_dado == 0)
            item->tipo_dado = "void";
        else if (tipo_dado == 1)
            item->tipo_dado = "int";
        else if (tipo_dado == 2)
            item->tipo_dado = "booleano";
        //item->param = param;
        if (param == 1)
            item->param = "sim";
        else if (param == 0)
            item->param = "não";
        else if (param == -1)
            item->param = "-";
         //item->vetor = vetor;
        if (vetor == 1)
            item->vetor = "sim";
        else if (vetor == 0)
            item->vetor = "não";
        else if (vetor == -1)
            item->vetor = "-";
        item->lines = (LineList)malloc(sizeof(struct LineListRec));
        item->lines->lineno = lineno;
        item->indice = indice;
        item->lines->next = NULL;
        item->next = hashTable[h];
        hashTable[h] = item;
    }
    /* found in table, so just add line number */
    else { 
        LineList t = item->lines;
        while (t->next != NULL) 
            t = t->next;
        t->next = (LineList)malloc(sizeof(struct LineListRec));
        t->next->lineno = lineno;
        t->next->next = NULL;
    }
}

int busca_indice(char* nome_id, char* escopo, char* tipo_id) { 
    int h = hash(nome_id, escopo);
    BucketList item =  hashTable[h];

    while ((item != NULL) && ((strcmp(tipo_id, item->tipo_id) != 0) || (strcmp(nome_id, item->nome_id) != 0) || (strcmp(escopo, item->escopo) != 0)))
        item = item->next;
        
    if (item == NULL)
        return -1;
  else
      return item->indice;
}

char* busca_tipo_dado(char* nome_id, char* escopo) { 
    int h = hash(nome_id, escopo);	
    BucketList item =  hashTable[h];
    while ((item != NULL) && (strcmp(nome_id, item->nome_id) != 0) && (strcmp(escopo, item->escopo) != 0))
        item = item->next;
    if (item == NULL) 
        return "null";
    else 
          return item->tipo_dado;
}

char* busca_tipo_id(char* nome_id, char* escopo) { 
    int h = hash(nome_id, escopo);	
    BucketList item =  hashTable[h];
    while ((item != NULL) && (strcmp(nome_id, item->nome_id) != 0) && (strcmp(escopo, item->escopo) != 0))
        item = item->next;
    if (item == NULL) 
        return "null";
    else 
        return item->tipo_id;
}

int busca_escopo(char* nome_id, char* escopo) { 
    int h = hash(nome_id, escopo);	
    BucketList item =  hashTable[h];
    while ((item != NULL) && (strcmp(nome_id, item->nome_id) != 0) && (strcmp(escopo, item->escopo) != 0))
        item = item->next;
    if (item == NULL) 
        return -1;
    else 
        return 1;
}

char* eh_parametro(char* nome_id, char *escopo) {
    int h = hash(nome_id, escopo);
    BucketList item = hashTable[h];
    while ((item != NULL) && (strcmp(nome_id, item->nome_id) != 0) && (strcmp(escopo, item->escopo) != 0))
        item = item->next;
    if (item == NULL) 
        return "null";
    else 
        return item->param;
}

char* eh_vetor(char* nome_id, char *escopo) {
    int h = hash(nome_id, escopo);
    BucketList item = hashTable[h];
    while ((item != NULL) && (strcmp(nome_id, item->nome_id) != 0) && (strcmp(escopo, item->escopo) != 0))
        item = item->next;
    if (item == NULL) 
        return "null";
    else 
        return item->vetor;
}

void imprime_tabela(FILE * listing) { 
    int i;
    fprintf(listing,"Índice na tabela  Nome Identificador  Tipo Identificador  Parâmetro  Escopo  Tipo Dado  Linhas\n");
    fprintf(listing,"----------------  ------------------  ------------------  ---------  ------  ---------  --------\n");
    for (i = 0; i < SIZE; i++) { 
        if (hashTable[i] != NULL) {
            BucketList item = hashTable[i];
            while (item != NULL) {
                LineList t = item->lines;
                fprintf(listing,"%-18d", item->indice);
                fprintf(listing,"%-20s", item->nome_id);
		        fprintf(listing,"%-20s  ", item->tipo_id);
                fprintf(listing,"%-11s", item->param);
                fprintf(listing,"%-8s", item->escopo);
		        fprintf(listing,"%-11s", item->tipo_dado);	
                while (t != NULL) { 
                    fprintf(listing,"%2d ",t->lineno);
                    t = t->next;
                }
                fprintf(listing,"\n");
                item = item->next;
            }
        }
    }
}