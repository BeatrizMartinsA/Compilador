#ifndef _SYMTAB_H_
#define _SYMTAB_H_

void insere_tabela(int indice, char * nome_id, char *escopo, char * tipo_id, int tipo_dado, int param, int vetor, int lineno);
int busca_indice(char* nome_id, char* escopo, char* tipo_id);
char* busca_tipo_dado(char* nome_id, char* escopo);
char* busca_tipo_id(char* nome_id, char* escopo);
int busca_escopo(char* nome_id, char* escopo);
char* eh_parametro(char* nome_id, char *escopo);
char* eh_vetor(char* nome_id, char *escopo);
void imprime_tabela(FILE * listing);

#endif
