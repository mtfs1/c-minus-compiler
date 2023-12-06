#include "parser.h"
#include "semantic.h"
#include <stdlib.h>


node *get(int type) {
  node *n = (node *)calloc(1, sizeof(node));
  n->type = type;
  return n;
}

node *bin_op(node *n1, node *op, node *n2) {
  n1->s = n2;
  op->c = n1;
  return op;
}

node *app(node *list, node *n) {
  node *ptr = list;
  while(ptr->s)
    ptr = ptr->s;
  ptr->s = n;

  return list;
}

node *capp(node *list, node *n) {
  if(list) {
    return app(list, n);
  }

  return n;
}

node *new_ter_node(int type, node *n1, node *n2, node *n3) {
  node *parent = get(type);
  n2->s = n3;
  n1->s = n2;
  parent->c = n1;

  return parent;
}

node *new_quat_node(int type, node *n1, node *n2, node *n3, node *n4) {
  node *parent = get(type);
  n3->s = n4;
  n2->s = n3;
  n1->s = n2;
  parent->c = n1;

  return parent;
}

struct Scope *get_scope_node(int start_line, int start_char,
     int end_line, int end_char) {

  struct Scope *scope = (struct Scope *)malloc(sizeof(struct Scope));
  scope->start_line = start_line;
  scope->start_char = start_char;
  scope->end_line = end_line;
  scope->end_char = end_char;

  return scope;
}

int indent = 0;
char op[] = {'*', '/', '+', '-'};
char op2[] = {'<', '>'};

void indentation(){
  int i;
  for(i=0;i<indent*INDENT_SIZE;i++)
    printf(" ");
}

void showAST(node *tree){
  indent++;
  while(tree != NULL){
    indentation();
    switch (tree->type)
    {
    case N_ID:
      printf("id: %s\n", (char*)tree->val);
      break;
    case N_NUM:
      printf("number: %d\n", (int)tree->val);
      break;
    case N_EXPR:
      printf("expression\n");
      break;
    case N_ARGS:
      if(tree->c != NULL)
        printf("args:\n");
      else
        printf("no args\n");
      break;
    case N_ACTV:
      printf("call to function\n");
      break;
    case N_MULT: case N_DIV: case N_PLUS: case N_MIN:
      printf("Operador: %c\n", op[tree->type-6]);
      break;
    case N_DIFF:
      printf("Relational Operador: !=\n");
      break;
    case N_EQU:
      printf("Relational Operador: ==\n");
      break;
    case N_GET:
      printf("Relational Operador: >=\n");
      break;
    case N_LET:
      printf("Relational Operador: <=\n");
      break;
    case N_GT: case N_LT:
      printf("Relational Operador: %c\n", op2[tree->type-13]);
      break;
    case N_IDX:
      printf("vector\n");
      break;
    case N_ATR:
      printf("atribuition\n");
      break;
    case N_RET:
      printf("return\n");
      break;
    case N_WHL:
      printf("while loop\n");
      break;
    case N_IF:
      printf("conditional\n");
      break;
    case N_CPD:
      printf("begin scope\n");
      break;
    case N_DCL:
      if(tree->c != NULL)
        printf("param:\n");
      break;
    case N_ADCL:
      if(tree->c != NULL)
        printf("param:\n");
      break;
    case N_FNDCL:
      printf("function declaration\n");
      break;
    case N_VOID:
      printf("Type VOID\n");
      break;
    case N_INT:
      printf("Type INT\n");
      break;
    case N_PAR:
      if(tree->c != NULL)
        printf("begin parameters\n");
      else
        printf("no parameters\n");
      break;
    default:
      printf("Unknown node\n");
      break;
    }
    showAST(tree->c);
    tree = tree->s;
  }
  indent--;
}


void printSymbolTable(struct SymbolTable *symbol_table){
  int i;
  struct SymbolTableNode *ptr;
  printf("----------------------------------------------\n");
  printf("name      | type      | val       | scope\n");
  for(i=0;i<BINS;i++){
    if(&symbol_table->hash_table[i] != NULL){
      ptr = &symbol_table->hash_table[i];
      while(ptr){
        if(!ptr->name){
          printf("%-8d %-8d\n",ptr->type,(int)ptr->val);
        }
        ptr = ptr->next;
      }
    }
  }

}