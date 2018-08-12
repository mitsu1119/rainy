/* execute.c */
#include <stdint.h>
#include "AST.h"
#include "execute.h"
#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>

// ローカル変数の数格納
int idp = 0;

// ローカル変数のリスト
Id *localId[MAX_LOCALVAR];

// リターン用
jmp_buf *retbuf;

// 関数の返り値格納用
int_fast32_t returnVal;

// エラー用
void error(char *str) {
	fprintf(stderr, "ERROR: %s\n", str);
	exit(1);
}

// 関数定義
void defineFunc(Id *func, AST *parameters, AST *body) {
	func->params = parameters;
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
	int i;
	// ローカルの方
	for(i = idp-1; i>=0; i--) {
		if(localId[i] == var) {
			return localId[i]->ival;
		}
	}
	return var->ival;
}

// 変数の代入
int_fast32_t setVal(Id *var, int_fast32_t val) {
	int i;
	// ローカルの方を見る
	for(i = idp-1; i>=0; i--) {
		if(localId[i] == var) {
			localId[i]->ival = val;
			return val;
		}
	}
	// グローバル
	var->ival = val;
	return val;
}

int_fast32_t exeExp(AST* p);
void exeBlock(AST *body);

// ローカル変数の宣言
void declareLocalVar(Id *var, AST *initval) {
	if(initval == NULL) {
		fprintf(stderr, "variable %s is not declared\n", var->name);
		exit(1);
	} else {
		localId[idp] = var;
		localId[idp++]->ival = exeExp(initval);
	}
}

// 文の実行
void exeStatement(AST *p) {
	if(p == NULL) return;
	switch(p->type) {
		case BLOCK_T:
			exeBlock(p->left);
			break;
		case RETURN_T:
			exeReturn(p->left);
			break;
		case LOCALVAR_T:
			declareLocalVar(getId(p->left), p->right);
			break;
		default:
			exeExp(p);
	};
}

// ブロックの実行(ローカル変数なし)
void exeBlock(AST *body) {
	AST *stat = body;
	int idpsave = idp;

	while(stat != NULL) {
		exeStatement(getList(stat, 0));
		stat = getNext(stat);
	}
	idp = idpsave;
	return;
}

// return の実行(返り値のみ)
void exeReturn(AST *retv) {
	returnVal = exeExp(retv);
	longjmp(*retbuf, 1);				// callfunc に戻る
	error("ret failed\n");
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
int_fast32_t exeCall(Id *func, AST *arguments) {
	jmp_buf retme;
	jmp_buf *retme_save;
	int_fast32_t retval;	// 返り値

	printf("calling %s function...\n", func->name);
	retme_save = retbuf;
	retbuf = &retme;

	// 引数の受け取り
	AST *args = arguments;
	AST *params = func->params;
	while(params != NULL) {
		if(args == NULL) {
			printf("invalid calling function %s\n", func->name);
			exit(1);
		}
		declareLocalVar(getId(getList(params, 0)), getList(args,0));
		params = getNext(params);
		args = getNext(args);
	}
	if(args != NULL) {
		printf("invalid calling function %s\n", func->name);
		exit(1);
	}

	if(setjmp(retbuf) != 0) {	// 帰って来たとき
		retval = returnVal;
	} else {
		exeStatement(func->funcbody);
	}
	retbuf = retme_save;
	return retval;
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
			return setVal(getId(p->left), exeExp(p->right));
		case ADD_T:
			return exeExp(p->left)+exeExp(p->right);
		case SUB_T:
			return exeExp(p->left)-exeExp(p->right);
		case MUL_T:
			return exeExp(p->left)*exeExp(p->right);
		case DIV_T:
			return exeExp(p->left)/exeExp(p->right);
		case CALL_T:
			return exeCall(getId(p->left), p->right);	// p->right は引数のリストのAST
		case PRINTLN_T:
			printFunc(p->left);
			return 0;
		default:
			error("unknown statement");
	};
}
