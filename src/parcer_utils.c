#include "parser.h"


node *get(int type) {
  return (node *)calloc(1, sizeof(node));
}

node *bin_op(node *n1, node *op, node *n2) {
  n1->s = n2;
  op->c = n1;
  return n1;
}

node *app(node *list, node *n) {
  node *ptr = list;
  while(ptr->s)
    ptr = ptr->s;
  ptr->s = n;

  return list;
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
