%{
#define YYPARSER    
#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"
#define YYSTYPE Node *

static Node * Tree;
static int yylex(void);
void yyerror(char *);
%}

%start programa
%token FIM ERRO
%token IF ELSE WHILE INT VOID RETURN ID NUM
%token SOMA SUB MULT DIV IGUAL VIRGULA PONTOVIRGULA
%token MENOR MAIOR MENORIGUAL MAIORIGUAL EHIGUAL DIFERENTE
%token ABREPARENTESE FECHAPARENTESE ABRECOLCHETE FECHACOLCHETE ABRECHAVE FECHACHAVE
%expect 1

%%

programa:	
 	declaracao_lista{
 		Tree = $1;
 	};

declaracao_lista:
	declaracao_lista declaracao {
		YYSTYPE t = $1;
		if (t != NULL){
			while (t->irmao != NULL){
				t = t->irmao;
			}
			t->irmao = $2;
			$$ = $1;
		}
		else 
			$$ = $2;
	} 
	| declaracao {
		$$ = $1;
	};

declaracao:
	var_declaracao {
		$$ = $1;
	} 
	| fun_declaracao{
		$$ = $1;
	};

var_declaracao:
	tipo_especificador ident PONTOVIRGULA {
		$$ = $1;
		$$->filho[0] = $2;
		$2->tipoNo = declaration;
		$2->tipo.dcl = tvariavel;
		$2->typeExp = $1->typeExp;
	} 
	| tipo_especificador ident ABRECOLCHETE num FECHACOLCHETE PONTOVIRGULA{
		$$ = $1;
		$$->filho[0] = $2;
		$2->tipoNo = declaration;
		$2->tipo.dcl = tvector;
		$2->atr.len = $4->atr.val;
		$2->typeExp = $1->typeExp;
	};

tipo_especificador:
	INT {
		$$ = expressionNode(type);
		$$->typeExp = tint;
		$$->atr.nome = "Integer";
	}
	| VOID{
		$$ = expressionNode(type);
		$$->typeExp = tvoid;
		$$->atr.nome = "Void";
	};

fun_declaracao:
	tipo_especificador ident ABREPARENTESE params FECHAPARENTESE composto_decl{
		$$ = $1;
		$$->filho[0] = $2;
		$2->filho[0] = $4;
		$2->filho[1] = $6;
		$2->tipoNo = declaration;
		$2->tipo.dcl = tfunction;
		$2->typeExp = $1->typeExp;
	} ;

params:
	param_lista{
		$$ = $1;
	} | VOID {
		
	};

param_lista:
	param_lista VIRGULA param {
		YYSTYPE t = $1;
		if (t != NULL){
			while (t->irmao != NULL){
				t = t->irmao;
			}
			t->irmao = $3;
			$$ = $1;
		}
		else 
			$$ = $1;
	} 
	| param{
		$$ = $1;
	};

param:
	tipo_especificador ident {
		$$ = $1;
		$$->filho[0] = $2;
		$2->tipoNo = declaration;
		$2->tipo.dcl = tparam;
		$2->typeExp = $1->typeExp;
	} 
	| tipo_especificador ident ABRECOLCHETE FECHACOLCHETE {
		$$ = $1;
		$$->filho[0] = $2;
		$2->tipoNo = declaration;
		$2->tipo.exp = vector;
		$2->tipo.dcl = tparam;
		$2->typeExp = $1->typeExp;
	};
	

composto_decl:
	ABRECHAVE local_declaracoes statement_lista FECHACHAVE {
		YYSTYPE t = $2;
		if (t != NULL){
			while (t->irmao != NULL){
				t = t->irmao;
			}
			t->irmao = $3;
			$$ = $2;
		}
		else 
			$$ = $3;
	}
	|   ABRECHAVE local_declaracoes FECHACHAVE{
    		$$ = $2;
		
		}
	|   ABRECHAVE statement_lista FECHACHAVE{
		
				$$ = $2;
			
		}
	|   ABRECHAVE FECHACHAVE{
		};
	


local_declaracoes:
	local_declaracoes var_declaracao {
		YYSTYPE t = $1;
		if (t != NULL){
			while (t->irmao != NULL){
				t = t->irmao;
			}
			t->irmao = $2;
			$$ = $1;
		}
		else
			$$ = $2;
	} 
	| var_declaracao{

		$$ = $1;

	};


statement_lista:
    statement_lista statement {
    	YYSTYPE t = $1;
		if (t != NULL){
			while (t->irmao != NULL){
				t = t->irmao;
			}
			t->irmao = $2;
			$$ = $1;
		}
		else 
			$$ = $2;
    } 
    | statement
	{
		$$ = $1;
	};



statement:
    expressao_decl {
    	$$ = $1;
    } 
    | composto_decl{
    	$$ = $1;
    } 
    | selecao_decl{
    	$$ = $1;
    } 
    | iteracao_decl{
    	$$ = $1;
    }
	| retorno_decl{
    	$$ = $1;
    };



expressao_decl:
    expressao PONTOVIRGULA{
    	$$ = $1;
    } 
    | PONTOVIRGULA{

    };



selecao_decl:
    IF ABREPARENTESE expressao FECHAPARENTESE statement{
    	$$ = declarationNode(tif);
    	$$->filho[0]  =$3;
    	$$->filho[1] = $5;
    } 
    | IF ABREPARENTESE expressao FECHAPARENTESE statement ELSE statement{
    	$$ = declarationNode(tif);
    	$$->filho[0] = $3;
    	$$->filho[1] = $5;
    	$$->filho[2] = $7;
    };


iteracao_decl:
    WHILE ABREPARENTESE expressao FECHAPARENTESE statement{
    	$$ = declarationNode(twhile);
    	$$->filho[0] = $3;
    	$$->filho[1] = $5;
    };


retorno_decl:
    RETURN PONTOVIRGULA{
    	$$ = declarationNode(treturn);
    } 
    | RETURN expressao PONTOVIRGULA{
    	$$ = declarationNode(treturn);
    	$$->filho[0] = $2;
    };


expressao:
    var IGUAL expressao{
    	$$ = declarationNode(tigual);
    	$$->filho[0] = $1;
    	$$->filho[1] = $3;
    } 
    | simples_expressao{
    	$$ = $1;
    };


var:
    ident{
    	$$ = $1;
    } 
    | ident ABRECOLCHETE expressao FECHACOLCHETE{
    	$$ = $1;
    	$$->filho[0] = $3;
    	$$->tipo.exp = vector;
    };


simples_expressao:
    soma_expressao relacional soma_expressao{
    	$$ = $2;
    	$$->filho[0] = $1;
    	$$->filho[1] = $3;
    }
    | soma_expressao{
    	$$ = $1;
    };

    
relacional:
    MENORIGUAL{
    	$$ = expressionNode(op);
    	$$->atr.op = MENORIGUAL;

    }
    | MAIOR{
		$$ = expressionNode(op);
    	$$->atr.op = MAIOR;
    } 
    | MENOR{
    	$$ = expressionNode(op);
    	$$->atr.op = MENOR;
    } 
    | MAIORIGUAL{
    	$$ = expressionNode(op);
    	$$->atr.op = MAIORIGUAL;

    } 
    | EHIGUAL{
    	$$ = expressionNode(op);
    	$$->atr.op = EHIGUAL;
    } 
    | DIFERENTE{
    	$$ = expressionNode(op);
    	$$->atr.op = DIFERENTE;
    };



soma_expressao:
    soma_expressao soma termo{
    	$$ = $2;
    	$$->filho[0] = $1;
    	$$->filho[1] = $3;
    } 
    | termo{
    	$$ = $1;
    };

soma:
    SOMA{
    	$$ = expressionNode(op);
    	$$->atr.op = SOMA;
    } 
    | SUB{
    	$$ = expressionNode(op);
    	$$->atr.op = SUB;
    };

termo:
    termo mult fator {
    	$$ = $2;
    	$$->filho[0] = $1;
    	$$->filho[1] = $3;
    }
	| fator{
    	$$ = $1;
    };

mult:
    MULT{
    	$$ = expressionNode(op);
    	$$->atr.op = MULT;
    } 
    | DIV{
    	$$ = expressionNode(op);
    	$$->atr.op = DIV;
    };

fator:
    ABREPARENTESE expressao FECHAPARENTESE{
    	$$ = $2;
    } 
    | var{
    	$$ = $1;
    } 
    | ativacao{
    	$$ = $1;
    } 
    | num{
    	$$ = $1;
    };

ativacao:
    ident ABREPARENTESE arg_lista FECHAPARENTESE{
    	$$ = $1;
    	$$->filho[0] = $3;
    	$$->tipoNo = declaration;
    	$$->tipo.dcl = tcall;
    }
	| ident ABREPARENTESE FECHAPARENTESE{

		$$ = $1;
		$$->tipoNo = declaration;
		$$->tipo.dcl = tcall;

	};

arg_lista: 
	arg_lista VIRGULA expressao{
	 	YYSTYPE t = $1;
		if (t != NULL){
			while (t->irmao != NULL){
				t = t->irmao;
			}
			t->irmao = $3;
			$$ = $1;
		}
		else 
			$$ = $3;	
	} 
	| expressao{
		$$ = $1;
	};

ident:
	ID {
		$$ = expressionNode(id);
		$$->atr.nome = copyString(tokenString);
	};

num:
	NUM {
		$$ = expressionNode(constante);
		$$->atr.val = atoi(tokenString);
	}
%%

void yyerror(char* message){
    if(yychar != FIM){	
    	fprintf(listing, "Erro sintático na linha %d: %s\n", lineno, message);
    	fprintf(listing, "Token atual: ");
    	printToken(yychar, tokenString);
    	Error = TRUE;
	}
}

static int yylex(void){
    return getToken();
}

Node * parse(void){
    yyparse();
    return Tree;
}
