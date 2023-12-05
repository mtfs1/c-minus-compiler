#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "semantic.h"


struct SymbolTableNode *new_symbol(char *name, struct Scope *scope,
     int type, int vtype, size_t val) {

  struct SymbolTableNode *node =
    (struct SymbolTableNode *)malloc(sizeof(struct SymbolTableNode));

  size_t effective_val = val;
  if(type == S_TYPE_FUNCTION) {
    effective_val = (size_t)malloc(sizeof(struct VectorParams));
    *(struct VectorParams *)effective_val = new_vec_params();
  }

  node->name = name;
  node->scope = scope;
  node->type = type;
  node->vtype = vtype;
  node->val = (size_t)effective_val;
  node->vector_position = new_vec_position();

  return node;
}

struct SymbolTable *new_symbol_table() {
  return (struct SymbolTable *)calloc(1, sizeof(struct SymbolTable));
}

int contains_scope(struct Scope *scope_1, struct Scope *scope_2) {
  if(!scope_1)
    return 1;

  if(scope_1->start_line > scope_2->start_line
      || scope_1->end_line < scope_2->end_line) {
    return 0;
  }

  if(scope_1->start_line < scope_2->start_line
      && scope_1->end_line > scope_2->end_line) {
    return 1;
  }

  if(scope_1->start_line == scope_2->start_line
      && scope_1->start_char > scope_2->start_char) {
    return 0;
  }

  if(scope_1->end_line == scope_2->end_line
      && scope_1->end_char < scope_2->end_char) {
    return 0;
  }

  return 1;
}

int contains_tok(struct Scope *scope, int line, int ch) {
  if(!scope)
    return 1;

  if(line < scope->start_line || line > scope->end_line)
    return 0;

  if(line == scope->start_line && ch < scope->start_char)
    return 0;

  if(line == scope->end_line && ch > scope->end_char)
    return 0;

  return 1;
}

struct SymbolTableNode *insert_symbol(struct SymbolTable *symbol_table,
    char *name, struct Scope *scope, int type, int vtype, size_t val) {

  struct SymbolTableNode *node = new_symbol(name, scope, type, vtype, val);
  int hash_val = hash(name);

  struct SymbolTableNode *ptr = &symbol_table->hash_table[hash_val];
  struct SymbolTableNode *previous = ptr;

  if(!ptr->name) {
    *ptr = *node;
    free(node);
    return ptr;
  }

  while(ptr) {
    if(strcmp(ptr->name, node->name) == 0
        && ((ptr->scope == NULL && node->scope == NULL)
        || (ptr->scope->start_line == node->scope->start_line
          && ptr->scope->start_char == node->scope->start_char
          && ptr->scope->end_line == node->scope->end_line
          && ptr->scope->end_char == node->scope->end_char))) {
      return NULL;
    }
    previous = ptr;
    ptr = ptr->next;
  }

  previous->next = node;

  return node;
}

struct SymbolTableNode *get_entry(struct SymbolTable *symbol_table,
    char *name, int line, int ch) {

  struct SymbolTableNode *ret = NULL;

  int hash_val = hash(name);
  struct SymbolTableNode *ptr = &symbol_table->hash_table[hash_val];

  while(ptr && ptr->name) {
    if(contains_tok(ptr->scope, line, ch)) {
      if(!ret)
        ret = ptr;

      if(contains_scope(ret->scope, ptr->scope)) {
        ret = ptr;
      }
    }
    ptr = ptr->next;
  }

  return ret;
}

void semantic_error(char *err, struct ASTNode *n) {
  struct ASTNode *node = n;
  if(n->type == N_ACTV)
    node = n->c;

  printf("SEMANTIC ERROR: %s # LINE: %d CHAR: %d\n", err,
    node->line_num, node->char_num);
  exit(1);
}

int is_type_spec(struct ASTNode *node) {
  switch (node->type) {
    case N_INT:
      return 1;
    case N_VOID:
      return 1;
  }
  return 0;
}

int is_arithmetic_op(struct ASTNode *node) {
  return node->type == N_MULT || node->type == N_DIV
    || node->type == N_PLUS || node->type == N_MIN;
}

int is_relational_op(struct ASTNode *node) {
  return node->type == N_GET || node->type == N_GT || node->type == N_LT
    || node->type == N_LET || node->type == N_EQU || node->type == N_DIFF;
}

int is_int_equivalent(struct ASTNode *node, struct SymbolTable *symbol_table) {
  if(node->type == N_NUM)
    return 1;

  if(node->type == N_ID) {
    struct SymbolTableNode *entry = get_entry(symbol_table,
      (char *)node->val, node->line_num, node->char_num);

    if(!entry)
      return 0;

    if(entry->type != S_TYPE_VARIABLE)
      return 0;

    return entry->vtype == S_VTYPE_INT;
  }

  if(node->type == N_ACTV) {
    struct SymbolTableNode *entry = get_entry(symbol_table,
      (char *)node->c->val, node->line_num, node->char_num);

    if(!entry)
      return 0;

    return entry->vtype == S_VTYPE_INT;
  }

  if(node->type == N_IDX) {
    struct SymbolTableNode *entry = get_entry(symbol_table,
      (char *)node->c->val, node->c->line_num, node->c->char_num);

    if(!entry)
      return 0;

    return entry->vtype == S_VTYPE_INT;
  }

  if(is_arithmetic_op(node))
    return 1;

  return 0;
}

void add_all_params(struct VectorParams *vec, struct ASTNode *params) {
  struct ASTNode *ptr = params->c;
  while(ptr) {
    append_vec_params(vec, ptr->c->type);
    ptr = ptr->s;
  }
}

void check_all_params(struct VectorParams *vec, struct ASTNode *node,
    struct SymbolTable *symbol_table) {

  struct ASTNode *ptr = node->c;
  for (int i = 0; i < vec->size; i++) {
    if(!ptr)
      semantic_error("Wrong number of arguments on activation", node);

    if(vec->vec[i] == S_VTYPE_INT && !is_int_equivalent(ptr, symbol_table)) {
      semantic_error("Wrong type of argument", node);
    }

    ptr = ptr->s;
  }

  if(ptr)
    semantic_error("Wrong number of arguments on activation", node);
}

void semantic_analisys(struct ASTNode *node, struct SymbolTable *symbol_table,
    struct Scope *scope, int identifier_validity) {

  if(node->type == N_FNDCL) {
    struct ASTNode *identifier = node->c->s;

    struct SymbolTableNode *entry = insert_symbol(symbol_table,
      (char *)identifier->val, scope, S_TYPE_FUNCTION, node->c->type, 0);

    if(!entry)
      semantic_error("Multiple declarations of function", node->c->s);

    append_vec_position(&entry->vector_position, identifier->line_num,
      identifier->char_num);
    add_all_params((struct VectorParams *)entry->val, node->c->s->s);
  }

  if(node->type == N_DCL) {
    struct ASTNode *identifier = node->c->s;

    struct SymbolTableNode *entry = insert_symbol(symbol_table,
      (char *)identifier->val, scope, S_TYPE_VARIABLE, node->c->type, 0);

    if(!entry)
      semantic_error("Multiple declarations of variable", node->c->s);

    append_vec_position(&entry->vector_position, identifier->line_num,
      identifier->char_num);
  }

  if(node->type == N_ADCL) {
    struct ASTNode *identifier = node->c->s;

    struct SymbolTableNode *entry = insert_symbol(symbol_table,
      (char *)identifier->val, scope, S_TYPE_ARRAY, node->c->type,
      node->c->s->s->val);

    if(!entry)
      semantic_error("Multiple declarations of variable", node->c->s);

    append_vec_position(&entry->vector_position, identifier->line_num,
      identifier->char_num);
  }

  if(node->c) {
    struct Scope *current_scope = scope;
    if(node->type == N_CPD)
      current_scope = (struct Scope *)node->val;
    if(node->type == N_FNDCL)
      current_scope = (struct Scope *)node->c->s->s->s->val;

    int iv = identifier_validity;
    if(node->type == N_CPD)
      iv = 1;
    semantic_analisys(node->c, symbol_table, current_scope, iv);
  }

  if(node->type == N_ID && identifier_validity) {
    struct SymbolTableNode *entry = get_entry(symbol_table,
      (char *)node->val, node->line_num, node->char_num);

    if(!entry)
      semantic_error("Identifier does not exist", node);

    append_vec_position(&entry->vector_position, node->line_num, node->char_num);
  }

  if(node->type == N_ATR) {
    char *name = (char *)node->c->val;
    if(node->c->type == N_IDX)
      name = (char *)node->c->c->val;

    struct SymbolTableNode *entry = get_entry(symbol_table, name,
      node->c->line_num, node->c->char_num);

    if(!entry)
      semantic_error("Variable does not exist", node);

    if(entry->vtype == S_VTYPE_INT
        && !is_int_equivalent(node->c->s, symbol_table)) {

      semantic_error("Assignment of non integer value to integer variable",
        node->c->s);
    }
  }

  if(node->type == N_ACTV) {
    struct SymbolTableNode *entry = get_entry(symbol_table,
      (char *)node->c->val, node->c->line_num, node->c->char_num);

    if(!entry)
      semantic_error("Function does not exist", node);

    check_all_params((struct VectorParams *)entry->val, node->c->s,
      symbol_table);
  }

  if(node->type == N_IF || node->type == N_WHL) {
    if(node->c->type == N_ATR)
      semantic_error("Assignment expression when expecting boolean or number",
        node->c->c);

    if(node->c->type == N_ACTV)
      if(!is_int_equivalent(node->c, symbol_table))
        semantic_error(
          "Non int returning function when expecting int or boolean expression",
          node->c);
  }

  if(node->type == N_IDX) {
    if(!is_int_equivalent(node->c->s, symbol_table))
      semantic_error("Indexing with non integer", node->c);
  }

  if(is_arithmetic_op(node)) {
    if(!is_int_equivalent(node->c, symbol_table))
      semantic_error("Operation between non integers", node->c);

    if(!is_int_equivalent(node->c->s, symbol_table))
      semantic_error("Operation between non integers", node->c->s);
  }

  if(is_relational_op(node)) {
    if(!is_int_equivalent(node->c, symbol_table))
      semantic_error("Relational between non integers", node->c);

    if(!is_int_equivalent(node->c->s, symbol_table))
      semantic_error("Relational between non integers", node->c->s);
  }

  if(node->s)
    semantic_analisys(node->s, symbol_table, scope, identifier_validity);
}

