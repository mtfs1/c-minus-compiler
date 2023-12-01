%{
#include "token.h"
#include "lexer.h"
#include "parser.h"

struct Buffer *buffer = NULL;
struct StringTable *string_table = NULL;

int line_num = 0;
int char_num = 0;

%}

%token ELSE 257
%token IF 258
%token INT 259
%token RETURN 260
%token VOID 261
%token WHILE 262
%token PLUS 263
%token '-'
%token '*'
%token '/'
%token '<'
%token LESS_EQUAL_THAN 268
%token '>'
%token GREATER_EQUAL_THAN 270
%token LOGICAL_EQUAL 271
%token LOGICAL_DIFFERENT 272
%token '='
%token ';'
%token ','
%token '('
%token ')'
%token '['
%token ']'
%token '{'
%token '}'
%token IDENTIFIER 282
%token NUMBER 283

%start program

%%

program: declaration-list;

declaration-list: declaration-list declaration
                  | declaration
                  ;

declaration: var-declaration
             | fun-declaration
             ;

var-declaration: type-specifier id ';'
                  | type-specifier id '[' num ']' ';'
                  ;

type-specifier: INT
                | VOID
                ;

fun-declaration: type-specifier id '(' params ')' composed-declaration

params: param-list
        | VOID
        ;

param-list: param-list ',' param
            | param
            ;

param: type-specifier id
       | type-specifier id '[' ']'
       ;

composed-declaration: '{' local-declarations statement-list '}'

local-declarations: local-declarations var-declaration
                    | %empty
                    ;

statement-list: statement-list statement
                | %empty
                ;

statement: expression-declaration
           | composed-declaration
           | selection-declaration
           | iteration-declaration
           | return-declaration
           ;

expression-declaration: expression ';'
                        | ';'
                        ;

selection-declaration: IF '(' expression ')' statement
                       | IF '(' expression ')' statement ELSE statement
                       ;

iteration-declaration: WHILE '(' expression ')' statement

return-declaration: RETURN ';'
                    | RETURN expression ';'
                    ;

expression: var '=' expression
            | simple-expression
            ;

var: id
     | id '[' expression ']'
     ;

simple-expression: sum-expression relational sum-expression
                   | sum-expression
                   ;

relational: LESS_EQUAL_THAN
            | '<'
            | '>'
            | GREATER_EQUAL_THAN
            | LOGICAL_EQUAL
            | LOGICAL_DIFFERENT
            ;

sum-expression: sum-expression sum term
                | term
                ;

sum: '+'
     | '-'
     ;

term: term multiplication term
      | factor
      ;

multiplication: '*'
                | '/'
                ;

factor: '(' expression ')'
        | var
        | activation
        | num
        ;

activation: id '(' args ')'

args: arg-list
      | %empty
      ;

arg-list: arg-list ',' expression
          | expression
          ;

id: IDENTIFIER

num: NUMBER

%%

int yylex() {
  struct Token tok = parse_token(buffer, string_table);

  if(tok.type == 0) {
    return YYEOF;
  }

  line_num = tok.line_number;
  char_num = tok.char_number;

  switch(tok.type) {
    case TOK_PLUS:
      return '+';

    case TOK_MINUS:
      return '-';

    case TOK_ASTERISK:
      return '*';

    case TOK_SLASH:
      return '/';

    case TOK_LESS_THAN:
      return '<';

    case TOK_GREATER_THAN:
      return '>';

    case TOK_EQUAL:
      return '=';

    case TOK_SEMI_COLON:
      return ';';

    case TOK_COMMA:
      return ',';

    case TOK_OPEN_BRACKET:
      return '(';

    case TOK_CLOSE_BRACKET:
      return ')';

    case TOK_OPEN_SQUARE_BRACKET:
      return '[';

    case TOK_CLOSE_SQUARE_BRACKET:
      return ']';

    case TOK_OPEN_CURLY_BRACKET:
      return '{';

    case TOK_CLOSE_CURLY_BRACKET:
      return '}';

  }

  return tok.type + 256;
}

int parser(struct Buffer *buffer_arg, struct StringTable *string_table_arg) {
  buffer = buffer_arg;
  string_table = string_table_arg;
  yyparse();
}

int yyerror(const char *s) {
  fprintf(stderr, "Error: %s # LINHA: %d CHAR: %d\n", s, line_num, char_num);
  return 0;
}

