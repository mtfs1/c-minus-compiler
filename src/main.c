#include <stdio.h>

#include "buffered_read.h"
#include "lexer.h"
#include "parser.h"
#include "string_table.h"

node* parser(struct Buffer *buffer_arg, struct StringTable *string_table_arg);


int main(int argc, char **argv) {
  if(argc != 2) {
    printf("ERROR: Wrong number of args\n");
    printf("Correct usage:\n");
    printf("  ./compiler c_minus_source_file\n");
    return -1;
  }

  FILE *fp;
  if((fp = fopen(argv[1], "r")) == NULL) {
    printf("ERROR: Wrong file path\n");
    return -1;
  }

  struct Buffer file_buffer = allocate_buffer(fp);
  struct StringTable *string_table = allocate_string_table();
  node *tree;

  /*
  while(!file_buffer.is_eof) {
    struct Token tok = parse_token(&file_buffer, string_table);
    if(tok.type == 26) {
      printf("token type %d identifier: %s\n", tok.type, (char*)tok.val);
      continue;
    }
    if(tok.type == 27) {
      printf("token type %d number: %d\n", tok.type, (int)tok.val);
      continue;
    }
    printf("token type %d\n", tok.type);
  }
  */
  tree = parser(&file_buffer, string_table);

  printf("\nParse Tree:\n");
  printTable(tree);

  return 0;
}

