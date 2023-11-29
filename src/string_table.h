#include <stdlib.h>

#ifndef STRING_TABLE
#define STRING_TABLE

#define BINS 16

struct HashTableNode {
  char *string;
  struct HashTableNode *next;
};

struct StringTable {
  char *arena;
  int pos;
  struct HashTableNode hash_table[BINS];
};

struct HashTableNode *allocate_hash_table_node(char *string);
struct StringTable *allocate_string_table();
char *insert_string(char *str, struct StringTable *string_table);

#endif

