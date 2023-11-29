#include "buffered_read.h"
#include "string_table.h"
#include "token.h"


struct Token parse_token(struct Buffer *buffer,
   struct StringTable *string_table);

