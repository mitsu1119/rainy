/* AST.h */
#include <stdint.h>

// AST のタイプ
enum ASTtype {
    LIST_T, NUM_T, I32_T, ID_T,
    EQ_T, ADD_T, SUB_T, MUL_T, DIV_T, LBRACKET_T, RBRACKET_T, AND_T, OR_T, XOR_T,
    EQEQ_T,
    CALL_T, IF_T, RETURN_T, PRINTLN_T, BLOCK_T, LOCALVAR_T
};

// ID 構造体 (変数など
typedef struct _Id {
    char *name;
    int_fast32_t ival;  // 32bit int
   struct _AST *funcbody;
   struct _AST *params;
} Id;

// AST 構造体
typedef struct _AST {
    enum ASTtype type;
    int ival;
    struct _Id *id;
    struct _AST *right, *left;
} AST;

#define MAX_ID 100  // ID の最大数

AST *makeAST(enum ASTtype type, AST *left, AST *right);
AST *makeNumAST(int num);

AST *makeId(char *name);
int searchId(char *name);
Id *findId(char *name);
Id *getId(AST *p);


AST *getList(AST *p, int n);
AST *getNext(AST *p);
AST *addList(AST *list, AST *p);;

#define makeList1(x1) makeAST(LIST_T, x1, NULL);

void declareVar(Id *id, AST *initval);
void defineFunc(Id *func, AST *parameters, AST *body);

void error(char *str);
