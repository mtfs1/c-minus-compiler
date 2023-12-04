#include "parser.h"
#include "string_table.h"
#include <stdlib.h>


#define S_TYPE_VARIABLE 1
#define S_TYPE_FUNCTION 2
#define S_TYPE_ARRAY 3

#define S_VTYPE_INT 26
#define S_VTYPE_VOID 25


struct SymbolTableNode {
  char *name;
  struct Scope *scope;
  int type;
  int vtype;
  void *val;
  struct SymbolTableNode *next;
};


struct SymbolTable {
  struct SymbolTableNode hash_table[BINS];
};

struct SymbolTable *new_symbol_table();
struct SymbolTableNode *insert_symbol(struct SymbolTable *symbol_table,
  char *name, struct Scope *scope, int type, int vtype, void *val);
struct SymbolTableNode *get_entry(struct SymbolTable *symbol_table,
  char *name, int line, int ch);

