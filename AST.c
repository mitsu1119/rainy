/* AST.c */
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "AST.h"

Id IDtable[MAX_ID]; // IDテーブル
int id_point = 0;	// 今のIDの数

// AST を生成
AST *makeAST(enum ASTtype type, AST *left, AST *right) {
	AST *p;
	p = (AST *)malloc(sizeof(AST));
	p->type = type;
	p->right = right;
	p->left = left;
	return p;
}

// 数値の AST ノードをもとに生成
AST *makeNumAST(int num) {
	AST *p;
	p = (AST *)malloc(sizeof(AST));
	p->type = NUM_T;
	p->ival = num;
	return p;
}

// IDテーブルから名前を探しインデックスを返す。なかったら -1
int searchId(char *name) {
	int i;
	for(i=0; i<id_point; i++) {
		if(strcmp(IDtable[i].name, name) == 0) {
			return i;
		}
	}
	return -1;
}

// IDテーブルから名前を検索し返す。なかったら生成
Id *findId(char *name) {
	int i;
	Id *id = NULL;
	int searchind = searchId(name);

	if(searchind != -1) id = &IDtable[searchind];
	else {
		id = &IDtable[id_point++];
		id->name = strdup(name);
	}
	return id;
}

// ID の生成
AST *makeId(char *name) {
	AST *p;
	p = (AST *)malloc(sizeof(AST));
	p->type = ID_T;
	p->id = findId(name);
	return p;
}

// ID の構文木から ID を取り出す
Id *getId(AST *p) {
	if(p->type != ID_T) {
		fprintf(stderr, "getID failed\n");
		exit(1);
	}
	return p->id;
}

// n 番目のリストを所得
AST *getList(AST *p, int n) {
	if(p->type != LIST_T) {
		fprintf(stderr, "bad list accessing get...\n");
		exit(1);
	}
	if(n > 0) return getList(p->right, n-1);
	else return p->left;
}

// 最初の要素をとったリストを返す
AST *getNext(AST *p) {
	if(p->type != LIST_T) {
		fprintf(stderr, "bad list accessing\n");
		exit(1);
	}
	return p->right;
}

// リストの追加
AST *addList(AST *list, AST *p) {
	AST *q;
	if(list == NULL) return makeAST(LIST_T, p, NULL);
	q = list;
	while(q->right != NULL) q = q->right;
	q->right = makeAST(LIST_T, p, NULL);
	return list;
}
