#include <stdlib.h>


#ifndef TOKEN
#define TOKEN

struct Token {
  int type;
  int line_number;
  int char_number;
  size_t val;
};

#define TOK_ELSE 1
#define TOK_IF 2
#define TOK_INT 3
#define TOK_RETURN 4
#define TOK_VOID 5
#define TOK_WHILE 6

#define TOK_PLUS 7
#define TOK_MINUS 8
#define TOK_ASTERISK 9
#define TOK_SLASH 10
#define TOK_LESS_THAN 11
#define TOK_LESS_EQUAL_THAN 12
#define TOK_GREATER_THAN 13
#define TOK_GREATER_EQUAL_THAN 14
#define TOK_LOGICAL_EQUAL 15
#define TOK_LOGICAL_DIFFERENT 16
#define TOK_EQUAL 17
#define TOK_SEMI_COLON 18
#define TOK_COMMA 19
#define TOK_OPEN_BRACKET 20
#define TOK_CLOSE_BRACKET 21
#define TOK_OPEN_SQUARE_BRACKET 22
#define TOK_CLOSE_SQUARE_BRACKET 23
#define TOK_OPEN_CURLY_BRACKET 24
#define TOK_CLOSE_CURLY_BRACKET 25

#define TOK_IDENTIFIER 26
#define TOK_NUMBER 27

#endif

