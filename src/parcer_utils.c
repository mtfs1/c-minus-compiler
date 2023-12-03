#include "parser.h"
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

