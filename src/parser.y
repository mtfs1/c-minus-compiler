%{
#include <stdlib.h>

#include "token.h"
#include "lexer.h"
#include "parser.h"

struct Buffer *buffer = NULL;
struct StringTable *string_table = NULL;

int line_num = 0;
int char_num = 0;
size_t val = 0;
node *last = NULL;

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

program: decl-list            { $$ = $1; }

decl-list: decl-list decl     { $$ = app($1, $2); }
           | decl             { $$ = $1; }
           ;

decl: var-decl                { $$ = $1; }
      | fun-decl              { $$ = $1; }
      ;

var-decl: type-spec id ';'            { $$ = get(N_DCL); bin_op($1, $$, $2); }
          | type-spec id'['num']' ';' { $$ = new_ter_node(N_ADCL, $1, $2, $3); }
          ;

type-spec: INT                { $$ = get(N_INT); }
           | VOID             { $$ = get(N_VOID); }
           ;

fun-decl: type-spec id '(' params ')' composed-decl
  { $$ = new_quat_node(N_FNDCL, $1, $2, $3, $4); }

params: param-list            { $$ = $1; }
        | VOID                { $$ = NULL; }
        ;

param-list: param-list ',' param      { $$ = app($1, $2); }
            | param                   { $$ = $1; }
            ;

param: type-spec id           { $$ = get(N_DCL); bin_op($1, $$, $2); }
       | type-spec id '[' ']' { $$ = get(N_ADCL); bin_op($1, $$, $2); }
       ;

composed-decl: '{'local-decls stmt-list'}'  { $$ = get(N_CPD); $$ = app($1, $2); }

local-decls: local-decls var-decl           { $$ = app($1, $2); }
             | %empty                       { $$ = NULL; }
             ;

stmt-list: stmt-list stmt     { $$ = app($1, $2); }
           | %empty           { $$ = NULL; }
           ;

stmt: expr-decl               { $$ = $1; }
      | composed-decl         { $$ = $1; }
      | if-decl               { $$ = $1; }
      | while-decl            { $$ = $1; }
      | return-decl           { $$ = $1; printf("[STMT][%p]\n", $1); }
      ;

expr-decl: expr ';'           { $$ = $1; }
           | ';'              { $$ = NULL; }
           ;

if-decl: IF '('expr')' stmt               { $$ = get(N_IF); bin_op($1, $$, $2); }
         | IF '('expr')' stmt ELSE stmt   { printf("%p %p %p\n", $1, $2, $3); $$ = new_ter_node(N_IF, $1, $2, $3); }

while-decl: WHILE '('expr')' stmt         { $$ = get(N_WHL); bin_op($1, $$, $2); }

return-decl: RETURN ';'          { $$ = get(N_RET); }
             | RETURN expr ';'   { $$ = get(N_RET); $$->c = $1; printf("[RET][%p %p]\n", $$, $1); }
             ;

expr: var '=' expr            { $$ = get(N_ATR); bin_op($1, $$, $2); }
      | simple-expr           { $$ = $1; printf("[EXPR][%p %p]\n", $$, $1); }
      ;

var: id                       { $$ = $1; }
     | id '[' expr ']'        { $$ = get(N_IDX); bin_op($1, $$, $2); }
     ;

simple-expr: sum-expr relational sum-expr   { $$ = bin_op($1, $2, $3); }
             | sum-expr                     { $$ = $1; }
             ;

relational: LESS_EQUAL_THAN         { $$ = get(N_LET); }
            | '<'                   { $$ = get(N_LT); }
            | '>'                   { $$ = get(N_GT); }
            | GREATER_EQUAL_THAN    { $$ = get(N_GET); }
            | LOGICAL_EQUAL         { $$ = get(N_EQU); }
            | LOGICAL_DIFFERENT     { $$ = get(N_DIFF); }
            ;

sum-expr: sum-expr sum term   { $$ = bin_op($1, $2, $3); }
          | term              { $$ = $1; }
          ;

sum: '+'                      { $$ = get(N_PLUS); }
     | '-'                    { $$ = get(N_MIN); }
     ;

term: term mult factor        { $$ = bin_op($1, $2, $3); }
      | factor                { $$ = $1; }
      ;

mult: '*'                     { $$ = get(N_MULT); }
      | '/'                   { $$ = get(N_DIV); }
      ;

factor: '(' expr ')'          { $$ = $1; }
        | var                 { $$ = $1; }
        | activation          { $$ = $1; }
        | num                 { $$ = $1; }
        ;

activation: id '(' args ')'   { $$ = get(N_ACTV); bin_op($1, $$, $2); }

args: arg-list                { $$ = get(N_ARGS); $$->c = $1; }
      | %empty                { $$ = NULL; }
      ;

arg-list: arg-list ',' expr   { $$ = app($1, $2); }
          | expr              { $$ = $1; }
          ;

id: IDENTIFIER                { $$ = get(N_ID); $$->val = val; }

num: NUMBER                   { $$ = get(N_NUM); $$->val = val; }

%%

int yylex() {
  struct Token tok = parse_token(buffer, string_table);

  if(tok.type == 0) {
    return YYEOF;
  }

  line_num = tok.line_number;
  char_num = tok.char_number;
  val = tok.val;

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

