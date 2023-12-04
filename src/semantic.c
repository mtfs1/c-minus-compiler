#include <stdlib.h>
#include <string.h>

#include "semantic.h"


struct SymbolTableNode *new_symbol(char *name, struct Scope *scope,
     int type, int vtype, void *val) {

  struct SymbolTableNode *node =
    (struct SymbolTableNode *)malloc(sizeof(struct SymbolTableNode));

  node->name = name;
  node->scope = scope;
  node->type = type;
  node->vtype = vtype;
  node->val = val;

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
    char *name, struct Scope *scope, int type, int vtype, void *val) {

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

