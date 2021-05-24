#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef YYPARSER

#include "parser.tab.h"

#define ENDFILE 0

#endif


#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define MAXRESERVED 6
#define MAXFILHOS 3

typedef int TokenType;

extern FILE* source; 
extern FILE* listing;
extern FILE* code; 

extern int lineno;

typedef enum
{
	declaration, expression
} TypeNode;

typedef enum
{
	tif, twhile, tigual, tvariavel, tvector, tfunction, tparam, tcall, treturn

} DeclarationType;

typedef enum
{
	op, constante, id, vector, type

} ExpressionID;

typedef enum
{
	tvoid, tint, tbool
} ExpressionType;



typedef struct noArvore
{ 
   struct noArvore * filho[MAXFILHOS];
   struct noArvore * irmao;
   int lineno;
   TypeNode tipoNo;

   union 
   { 
      DeclarationType dcl; 
      ExpressionID exp;
   } tipo;

   struct 
   { 
      TokenType op;
      int val;
      int len;
      char * nome; 
      char * escopo;
   } atr;

   ExpressionType typeExp; /* for type checking of exps */
} Node;

/* EchoSource = TRUE causes the source program to
 * be echoed to the listing file with line numbers
 * during parsing
 */
extern int EchoSource;

/* TraceScan = TRUE causes token information to be
 * printed to the listing file as each token is
 * recognized by the scanner
 */
extern int TraceScan;

/* TraceParse = TRUE causes the syntax tree to be
 * printed to the listing file in linearized form
 * (using indents for children)
 */
extern int TraceParse;

/* TraceAnalyze = TRUE causes symbol table inserts
 * and lookups to be reported to the listing file
 */
extern int TraceAnalyze;

/* TraceCode = TRUE causes comments to be written
 * to the TM code file as code is generated
 */
extern int TraceCode;

/* Error = TRUE prevents further passes if an error occurs */
extern int Error; 
#endif