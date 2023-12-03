#include <stdio.h>

#include "buffered_read.h"
#include "lexer.h"
#include "string_table.h"
#include "parser.h"


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

  struct ASTNode *ast = parser(&file_buffer, string_table);

  return 0;
}

