#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "token.h"
#include "trie.h"
#include "string_table.h"


int char_to_index(char ch) {
  if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
    return 0;

  if(ch >= '0' && ch <= '9')
    return 1;

  if(ch == ' ' || ch == '\n' || ch == '\t')
    return 2;

  char chars[] = {
    '+', '-', '*', '/', '<', '=', '>', '!',
    ';', ',', '(', ')', '[', ']', '{', '}',
  };

  for(int i = 0; i < sizeof(chars); i++) {
     if(ch == chars[i])
      return i + 3;
  }

  return -1;
}

void print_error(char *lexeme_buffer, int next_char_in_buffer, char ch,
    struct Buffer *input_file) {

  printf("ERRO LEXICO: ");
  fwrite(lexeme_buffer, sizeof(char), next_char_in_buffer, stdout);
  fputc(ch, stdout);
  printf(" LINHA: %d\n", input_file->line_number);
  exit(-1);
}

int final_state_to_token_type(int state) {
  int tokens[] = {
    0,
    TOK_IDENTIFIER,
    TOK_NUMBER,
    TOK_PLUS,
    TOK_MINUS,
    TOK_ASTERISK,
    TOK_SLASH,
    TOK_LESS_THAN,
    TOK_EQUAL,
    TOK_LESS_EQUAL_THAN,
    TOK_GREATER_THAN,
    TOK_GREATER_EQUAL_THAN,
    TOK_LOGICAL_EQUAL,
    0,
    TOK_LOGICAL_DIFFERENT,
    TOK_SEMI_COLON,
    TOK_COMMA,
    TOK_OPEN_BRACKET,
    TOK_CLOSE_BRACKET,
    TOK_OPEN_SQUARE_BRACKET,
    TOK_CLOSE_SQUARE_BRACKET,
    TOK_OPEN_CURLY_BRACKET,
    TOK_CLOSE_CURLY_BRACKET
  };

  return tokens[state];
}

struct Token parse_token(struct Buffer *input_file,
     struct StringTable *string_table) {

  char lexeme_buffer[32];
  int next_char_in_buffer = 0;

  int state = 0;

  int table[24][19] = {
    // {char digit separator + - * / < = > ! ; , ( ) [ ] { }}

    // initial state
    // 0
    {1, 2, 0, 3, 4, 5, 6, 7, 8, 10, 13, 15, 16, 17, 18, 19, 20, 21, 22},

    // parsing keyword or identifier
    // 1
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    // parsing number
    // 2
    {0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    // parsing plus
    // 3
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    // parsing minus
    // 4
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    // parsing asterisk
    // 5
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    // parsing slash
    // 6
    {0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    // parsing less than
    // 7
    {0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    // parsing equal
    // 8
    {0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    // parsing less or equal than
    // 9
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    // parsing greater than
    // 10
    {0, 0, 0, 0, 0, 0, 0, 0, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    // parsing greater or equal than
    // 11
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    // parsing logical equal
    // 12
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    // parsing exclamation mark
    // 13
    {-1, -1, -1, -1, -1, -1, -1, -1, 14, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},

    // parsing logical different
    // 14
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    // parsing semi colon
    // 15
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    // parsing comma
    // 16
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    // parsing open bracket
    // 17
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    // parsing close bracket
    // 18
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    // parsing open square bracket
    // 19
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    // parsing close square bracket
    // 20
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    // parsing open curly bracket
    // 21
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    // parsing open close bracket
    // 22
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    // parsing open comment
    // 23
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  };

  struct Trie *trie_of_keywords = initialize_trie_of_lexemes();
  struct Trie *trie_ptr = trie_of_keywords;

  int flag_ignore_initial_separators = 1;
  int flag_keyword = 1;

  int char_pos = -1;
  int line = -1;

  while(!input_file->is_eof) {
    char ch = get_next_char(input_file);
    int index = char_to_index(ch);

    if(index == 2 && flag_ignore_initial_separators) {
      consume_next_char(input_file);
      continue;
    }
    flag_ignore_initial_separators = 0;

    if(char_pos < 0) {
      char_pos = input_file->char_number_in_line;
      line = input_file->line_number;
    }

    if(index == -1)
      print_error(lexeme_buffer, next_char_in_buffer, ch, input_file);

    int next_state = table[state][index];

    if(next_state == -1)
      print_error(lexeme_buffer, next_char_in_buffer, ch, input_file);

    if(next_state == 0) {
      int token_type = final_state_to_token_type(state);
      if(trie_ptr->lex)
        token_type = trie_ptr->lex;

      size_t val = 0;
      if(token_type == 26) {
        lexeme_buffer[next_char_in_buffer] = '\0';
        val = (size_t)insert_string(lexeme_buffer, string_table);
      }

      if(token_type == 27) {
        lexeme_buffer[next_char_in_buffer] = '\0';
        val = atoi(lexeme_buffer);
      }

      struct Token tok = {
        .type = token_type,
        .char_number = char_pos,
        .line_number = line,
        .val = val
      };

      return tok;
    }

    if(next_state == 1) {
      if(flag_keyword && trie_ptr->children[to_trie_index(ch)])
        trie_ptr = trie_ptr->children[to_trie_index(ch)];
      else {
        flag_keyword = 0;
        trie_ptr = trie_of_keywords;
      }
    }

    if(next_state == 23) {
      state = 0;
      next_char_in_buffer = 0;

      char_pos = -1;
      line = -1;

      int flag_asterisk = 0;
      while(!input_file->is_eof) {
        ch = get_next_char(input_file);
        consume_next_char(input_file);

        if(flag_asterisk && ch == '/')
          break;

        if(ch == '*') {
          flag_asterisk = 1;
          continue;
        }

        flag_asterisk = 0;
      }
      flag_ignore_initial_separators = 1;
      continue;
    }

    lexeme_buffer[next_char_in_buffer] = ch;
    next_char_in_buffer++;

    consume_next_char(input_file);
    state = next_state;
  }

  struct Token tok = {0};
  return tok;
}

