/* execute.h */
#include <stdint.h>

// 変数関係
int_fast32_t getVal(Id *var);
int_fast32_t setVal(Id *var, int_fast32_t val);

// 実行系
int_fast32_t exeExp(AST* exp);
int_fast32_t exeCall(Id *func);	// 関数コール、引数なし
void exeBlock(AST *body);
void exeStatement(AST *p);
