#include "token.h"


struct Trie {
  struct Trie *children[26];
  int lex;
};

struct Trie *allocate_trie_node();
int insert_in_trie(struct Trie *trie, char *str, int lex, int i);
int to_trie_index(char ch);
struct Trie *initialize_trie_of_lexemes();

