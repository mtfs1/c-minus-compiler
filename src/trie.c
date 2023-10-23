#include <stdio.h>
#include <stdlib.h>

#include "token.h"
#include "trie.h"


struct Trie *allocate_trie_node() {
  struct Trie *node = (struct Trie *)calloc(1, sizeof(struct Trie));
  return node;
}

int to_trie_index(char ch) {
  int val = ch - 'a';
  if (val < 0 || val > 25)
    return -1;

  return val;
}

int insert_in_trie(struct Trie *trie, char *str, int lex, int i) {
  if(!str[i]) {
    trie->lex = lex;
    return 0;
  }

  int index = to_trie_index(str[i]);
  if(index < 0)
    return 1;

  if(!trie->children[index])
    trie->children[index] = allocate_trie_node();

  return insert_in_trie(trie->children[index], str, lex, i + 1);
}

struct Trie *initialize_trie_of_lexemes() {
  struct Trie *trie = allocate_trie_node();

  insert_in_trie(trie, "else", TOK_ELSE, 0);
  insert_in_trie(trie, "if", TOK_IF, 0);
  insert_in_trie(trie, "int", TOK_INT, 0);
  insert_in_trie(trie, "return", TOK_RETURN, 0);
  insert_in_trie(trie, "void", TOK_VOID, 0);
  insert_in_trie(trie, "while", TOK_WHILE, 0);

  return trie;
}

