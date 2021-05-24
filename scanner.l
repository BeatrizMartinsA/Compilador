%{
#include <stdlib.h>
#include <stdio.h>
#include "globals.h"
#include "util.h"
#include "scan.h"

char tokenString[MAXTOKENLEN+1];
static int yylex(void);
int yywrap ();
%}

LETRA [a-zA-Z]
DIGITO [0-9]

%%
"if"        return IF;
"else"      return ELSE;
"while"     return WHILE;
"int"       return INT; 
"void"      return VOID;
"return"    return RETURN;

"+"         return SOMA;
"-"         return SUB;
"*"         return MULT;
"/"         return DIV;
"="         return IGUAL;
"<"         return MENOR;
">"         return MAIOR;
"<="        return MENORIGUAL;
">="        return MAIORIGUAL;
"=="        return EHIGUAL;
"!="        return DIFERENTE;
"("         return ABREPARENTESE;
")"         return FECHAPARENTESE;
"["         return ABRECOLCHETE;
"]"         return FECHACOLCHETE;
"{"         return ABRECHAVE;
"}"         return FECHACHAVE;
","         return VIRGULA;
";"         return PONTOVIRGULA;
"/*"        { char c;
                do {
                    c = input();
                    if (c == EOF) break;
                    if (c == '\n') lineno++;
                    if (c == '*') {
                        if (input() == '/') break;
                    } 
                } while (1);
            }
<<EOF>>     return FIM;
\n          lineno++;
[ \t]+      

{LETRA}+    return ID;
{DIGITO}+   return NUM;

.                   return ERRO;
{LETRA}+{DIGITO}+   return ERRO;
{DIGITO}+{LETRA}+   return ERRO;

%%
int yywrap () {  };

TokenType getToken(void) { 
    static int firstTime = TRUE;
    TokenType currentToken;
    if (firstTime) {
        firstTime = FALSE;
        lineno++;
        yyin = source;
        yyout = listing;
    }
    currentToken = yylex();
    strncpy(tokenString, yytext, MAXTOKENLEN); 
    if (TraceScan) { 
        if (currentToken != FIM) {
            fprintf(listing,"\t%d: ", lineno);
            printToken(currentToken, tokenString);
        }
        if (currentToken == ERRO) {
            printf("\nERRO LÉXICO. LINHA: %d, ", lineno);
            printToken(currentToken, tokenString);
            Error = TRUE;
        }
    }
    return currentToken;
}