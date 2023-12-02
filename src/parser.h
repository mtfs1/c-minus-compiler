#include <stdlib.h>


typedef struct ASTNode {
  int type;
  size_t val;
  struct ASTNode *s;
  struct ASTNode *c;
} node;

#define YYPARSER
#define YYSTYPE node*

int yylex();
int yyerror(const char *s);


// utils
node *get(int type);
node *bin_op(node *n1, node *op, node *n2);
node *app(node *list, node *n);
node *capp(node *list, node *n);
node *new_ter_node(int type, node *n1, node *n2, node *n3);
node *new_quat_node(int type, node *n1, node *n2, node *n3, node *n4);

#define N_ID 1
#define N_NUM 2
#define N_EXPR 3
#define N_ARGS 4
#define N_ACTV 5
#define N_MULT 6
#define N_DIV 7
#define N_PLUS 8
#define N_MIN 9
#define N_DIFF 10
#define N_EQU 11
#define N_GET 12
#define N_GT 13
#define N_LT 14
#define N_LET 15
#define N_IDX 16
#define N_ATR 17
#define N_RET 18
#define N_WHL 19
#define N_IF 20
#define N_CPD 21
#define N_DCL 22
#define N_ADCL 23
#define N_FNDCL 24
#define N_VOID 25
#define N_INT 26
#define N_PAR 27

