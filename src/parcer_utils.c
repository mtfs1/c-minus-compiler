#include "parser.h"
#include <stdio.h>

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

int indent = 0;
char op[] = {'*', '/', '+', '-'};
char op2[] = {'<', '>'};

void indentation(){
  int i;
  for(i=0;i<indent*INDENT_SIZE;i++)
    printf(" ");
}

void printTable(node *tree){
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
      printTable(tree->c);
      break;
    case N_ARGS:
      if(tree->c != NULL){
        printf("args:\n");
        printTable(tree->c);
      } else {
        printf("no args\n");
      }
      break;
    case N_ACTV:
      printf("call to function\n");
      if(tree->c != NULL)
        printTable(tree->c);
      break;
    case N_MULT: case N_DIV: case N_PLUS: case N_MIN:
      printf("Operador: %c\n", op[tree->type-6]);
      printTable(tree->c);
      break;
    case N_DIFF:
      printf("Relational Operador: !=\n");
      printTable(tree->c);
      break;
    case N_EQU:
      printf("Relational Operador: ==\n");
      printTable(tree->c);
      break;
    case N_GET:
      printf("Relational Operador: >=\n");
      printTable(tree->c);
      break;
    case N_LET:
      printf("Relational Operador: <=\n");
      printTable(tree->c);
      break;
    case N_GT: case N_LT:
      printf("Relational Operador: %c\n", op2[tree->type-13]);
      printTable(tree->c);
      break;
    case N_IDX:
      printf("vector\n");
      printTable(tree->c);
      break;
    case N_ATR:
      printf("atribuition\n");
      printTable(tree->c);
      break;
    case N_RET:
      printf("return\n");
      if(tree->c != NULL)
        printTable(tree->c);
      break;
    case N_WHL:
      printf("while loop\n");
      printTable(tree->c);
      break;
    case N_IF:
      printf("conditional\n");
      if(tree->c != NULL)
        printTable(tree->c);
      break;
    case N_CPD:
      printf("begin scope\n");
      if(tree->c != NULL)
        printTable(tree->c);
      indentation();
      printf("end scope\n");
      break;
    case N_DCL:
      if(tree->c != NULL){
        printf("param:\n");
        printTable(tree->c);
      }
      break;
    case N_ADCL:
      if(tree->c != NULL){
        printf("param:\n");
        printTable(tree->c);
      }
      break;
    case N_FNDCL:
      printf("function declaration\n");
      if(tree->c != NULL)
        printTable(tree->c);
      break;
    case N_VOID:
      printf("Type VOID\n");
      break;
    case N_INT:
      printf("Type INT\n");
      break;
    case N_PAR:
      if(tree->c != NULL){
        printf("begin parameters\n");
        printTable(tree->c);
        indentation();
        printf("end parameters\n");
      }
      else{
        printf("no parameters\n");
      }
      break;
    default:
      printf("Unknown node\n");
      break;
    }
    tree = tree->s;
  }
  indent--;
}