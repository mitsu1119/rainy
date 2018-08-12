%token  NUM I32 ID PRINTLN RETURN GLOBAL
%{
	#include <stdint.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	#include "AST.h"

%}

%union {
	AST *ast;
}

%right ':'
%left '+' '-'
%left '*' '/'

%type <ast> parameter IDs block statements statement exp primary_exp args
%type <ast> ID NUM

%start program

%%
program:	/* empty */
		| ext_defs
		;

ext_defs: extdef
		| ext_defs extdef
		;

// 宣言や定義
extdef:GLOBAL I32 ID ':' exp ';'	// 変数宣言
		{ declareVar(getId($3), $5); }
		| I32 ID parameter block 	// 関数定義
		{ defineFunc(getId($2), $4); }
		;

// 関数の宣言するときのパラメータ
parameter: '(' ')'
				{ $$ = NULL; }
				| '(' IDs ')'
				{ $$ = $2; }
				;

IDs: ID
		{ makeList1($1); }
		| IDs ID
		{ addList($1, $2); }
		;

block: '{' statements '}'
	{ $$ = makeAST(BLOCK_T, $2, NULL); }
	;

statements: statement
		{ $$ = makeList1($1); }
		| statements statement
		{ $$ = addList($1, $2); }
		;

statement: /* empty */
		{ $$ = NULL; }
		| exp ';'
		{ $$ = $1; }
		| block
		{ $$ = $1; }
		| RETURN exp ';'
		{ $$ = makeAST(RETURN_T, $2, NULL); }
		| I32 ID ':' exp ';'
		{ $$ = makeAST(LOCALVAR_T, $2, $4); }
		;

exp: primary_exp
	| ID ':' exp
	{ $$ = makeAST(COLON_T, $1, $3); }
	| exp '+' exp
	{ $$ = makeAST(ADD_T, $1, $3);}
	| exp '-' exp
	{ $$ = makeAST(SUB_T, $1, $3);}
	| exp '*' exp
	{ $$ = makeAST(MUL_T, $1, $3);}
	| exp '/' exp
	{ $$ = makeAST(DIV_T, $1, $3);}
	;

primary_exp: ID
			| NUM
			| ID '(' ')' 		// 関数コール
			{ $$ = makeAST(CALL_T, $1, NULL); }
			| ID '(' args ')'
			{ $$ = makeAST(CALL_T, $1, $3); }
			| '(' exp ')'
			{ $$ = $2; }
			| PRINTLN '(' args ')'
			{ $$ = makeAST(PRINTLN_T, $3, NULL); }
			;

args: exp
	{ $$ = makeList1($1); }
	| args ',' exp
	{ $$ = addList($1, $3); }
	;

%%
#include "lexer.c"
