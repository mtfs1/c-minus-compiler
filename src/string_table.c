#include "string_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct HashTableNode *allocate_hash_table_node(char *string) {
  struct HashTableNode *node =
    (struct HashTableNode *) calloc(1, sizeof(struct HashTableNode));
  node->string = string;
  return node;
}

struct Response {
  int has;
  struct HashTableNode *ptr;
};

// Return struct "Response" that has two attributes: "has" and "ptr"
// "has" is positive if bin has string, negative otherwise
// if "has" is positive, "ptr" has a pointer to the node relative to
// the specified string, otherwise "ptr" has a pointer to the last
// node on the linked list, so the caller can insert a new node
//
struct Response bin_has_string(struct HashTableNode *first_node,
     char *string) {

  struct HashTableNode *ptr = first_node;
  struct HashTableNode *previous = first_node;

  int has = 0;
  while (ptr && ptr->string) {
    if(strcmp(string, ptr->string) == 0) {
      has = 1;
      break;
    }
    previous = ptr;
    ptr = ptr->next;
  }

  struct Response res;
  res.has = has;
  res.ptr = previous;
  if (has) {
    res.ptr = ptr;
  }

  return res;
}

int hash(char *str) {
  char *ptr = str;
  int hash_val = 0;
  while (*ptr) {
    hash_val ^= *ptr;
    ptr++;
  }
  return hash_val % BINS;
}

char *insert_string(char *str, struct StringTable *string_table) {
  int hash_val = hash(str);

  struct Response res = bin_has_string(
    &string_table->hash_table[hash_val], str);

  if(res.has) {
    return res.ptr->string;
  }

  char *string_ptr = &string_table->arena[string_table->pos];
  memcpy(string_ptr, str, strlen(str));
  string_table->pos += strlen(str) + 1;

  if(!res.ptr->string) {
    res.ptr->string = string_ptr;
    return string_ptr;
  }

  struct HashTableNode *node = allocate_hash_table_node(string_ptr);
  res.ptr->next = node;

  return string_ptr;
}

struct StringTable *allocate_string_table() {
  struct StringTable *ptr = calloc(1, sizeof(struct StringTable));
  ptr->arena = calloc(1024, sizeof(char));
  return ptr;
}

