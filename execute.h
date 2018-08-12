/* execute.h */
#include <stdint.h>

// ローカル変数関係
#define MAX_LOCALVAR 100

// 変数関係
int_fast32_t getVal(Id *var);
int_fast32_t setVal(Id *var, int_fast32_t val);

// ローカル変数関係
void declareLocalVar(Id *var, AST *initval);

// 実行系
int_fast32_t exeExp(AST* exp);
int_fast32_t exeCall(Id *func, AST *arguments);	// 関数コール
void exeBlock(AST *body);
void exeStatement(AST *p);
void exeReturn(AST *retv);
