#include "globals.h"
#include "util.h"
#include "parse.h"
#include "scan.h"
#include "symtab.h"
#include "analyze.h"
#include "code.h"
#include "cgen.h"

/* set NO_PARSE to TRUE to get a scanner-only compiler */
#define NO_PARSE FALSE
/* set NO_ANALYZE to TRUE to get a parser-only compiler */
#define NO_ANALYZE FALSE
/* set NO_CODE to TRUE to get a compiler that does not generate code*/
#define NO_CODE FALSE


int  lineno = 0;
FILE * source;
FILE * listing;
FILE * code;

int TraceCode = FALSE;
int TraceScan = TRUE;
int TraceParse = TRUE;
int TraceAnalyze = TRUE;
int Error = FALSE;


int main(int argc, char * argv[]) { 

    Node* syntaxTree;
    char pgm[120]; /* source code file name */
    
    if (argc != 2) { 
        fprintf(stderr,"Arquivo: %s <filename>\n",argv[0]);
        exit(1);
    }
    
    strcpy(pgm, argv[1]) ;
    
    source = fopen(pgm, "r");
    
    if (source == NULL) { 
        fprintf(stderr,"Arquivo %s não encontrado!\n",pgm);
        exit(1);
    }
 
    listing = stdout; /* send listing to screen */
    
    fprintf(listing,"\nCOMPILAÇÃO C-: %s\n",pgm);
    
    if(NO_PARSE) {
        while (getToken() != ENDFILE);
    }
    else {
        syntaxTree = parse();

        if (TraceParse) {
            fprintf(listing,"\nÁrvore Sintática:\n");
            printTree(syntaxTree);
        }
    
        if(!NO_ANALYZE) {

            if (!Error) { 
                if (TraceAnalyze) {
                    fprintf(listing,"\nConstruindo Tabela de Símbolos...\n");
                    constroi_tabela(syntaxTree);
                }

                if (TraceAnalyze && !Error) { 
                    fprintf(listing,"\nChecagem de Tipos...\n");
                    checagem_tipo(syntaxTree);
                }

                if (TraceAnalyze && !Error) 
                    fprintf(listing,"\nChecagem de Tipos Finalizada!\n");
            }
        }
    }

    if(!Error && !NO_CODE) {
        int fnlen = strcspn(pgm, ".");
        char * codefile;

        codefile = (char *) calloc(fnlen+4, sizeof(char));

        strncpy(codefile, pgm, fnlen);
        strcat(codefile, ".intermediario");
        
        code = fopen(codefile, "w");

        if (code == NULL) { 
            printf("Não foi possível abrir o arquivo %s\n", codefile);
            exit(1);
        }
        intermediarioGen(syntaxTree, codefile);
        fclose(code);
        fprintf(listing,"\nCompilação Finalizada\n");
    }

    if (Error) {
        fprintf(listing,"\nCompilação apresentou erros!\n");
    }

    fclose(source);
    return 0;
}