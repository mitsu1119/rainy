/* execute.c */
#include <stdint.h>
#include "AST.h"
#include "execute.h"
#include <stdlib.h>
#include <stdio.h>

// 関数の返り値格納用
int_fast32_t returnVal;

// エラー用
void error(char *str) {
	fprintf(stderr, "ERROR: %s\n", str);
	exit(1);
}

// 関数定義
void defineFunc(Id *func, AST *body) {
	func->funcbody = body;
}

// 変数の宣言
void declareVar(Id *id, AST *initval) {
	if(initval == NULL) {
		fprintf(stderr, "variable %s is not declared\n", id->name);
		exit(1);
	}
	id->ival = exeExp(initval);
}

// 変数から値を取り出す
int_fast32_t getVal(Id *var) {
	return var->ival;
}

// 変数の代入
int_fast32_t setVal(Id *var, int_fast32_t val) {
	var->ival = val;
	return val;
}


int_fast32_t exeExp(AST* p);
void exeBlock(AST *body);

// 文の実行
void exeStatement(AST *p) {
	if(p == NULL) return;
	switch(p->type) {
		case BLOCK_T:
			exeBlock(p->right);
			break;
		default:
			exeExp(p);
	};
}


// ブロックの実行(ローカル変数なし)
void exeBlock(AST *body) {
	for(; body != NULL; body = getNext(body)) {
		exeStatement(getList(body,0));
	}
	return;
}

// プリント
static void printFunc(AST *args) {
	AST *p;
	printf("calling println\n");
	p = getList(args, 0);
	//if(p->type != NUM_T) error("invalid format for println");
	printf("%s = %d\n", p->id->name, p->id->ival);
}

// 関数コール
int_fast32_t exeCall(Id *func) {
	printf("calling %s function...\n", func->name);
	exeStatement(func->funcbody);
	printf("success calling %s function !!\n", func->name);
	return 0;
}

// exp の計算
int_fast32_t exeExp(AST* p) {
	if(p == NULL) return 0;
	switch(p->type) {
		case NUM_T:
			return p->ival;
		case ID_T:
			return getVal(getId(p));
		case COLON_T:
			return setVal(getId(p->left), exeExp(getId(p->right)));
		case ADD_T:
			return exeExp(p->left)+exeExp(p->right);
		case SUB_T:
			return exeExp(p->left)-exeExp(p->right);
		case MUL_T:
			return exeExp(p->left)*exeExp(p->right);
		case DIV_T:
			return exeExp(p->left)/exeExp(p->right);
		case CALL_T:
			return exeCall(getId(p->left));
		case PRINTLN_T:
			printFunc(p->left);
			return 0;
		default:
			error("unknown statement");
	};
}
