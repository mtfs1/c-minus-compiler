#include <stdlib.h>

#ifndef VECTOR_POSITION
#define VECTOR_POSITION

struct Position {
  int line_num;
  int char_num;
};

struct VectorPos {
  int size;
  int capacity;
  struct Position *vec;
};

struct VectorPos new_vec_position();
void append_vec_position(struct VectorPos *vec, int line_num, int char_num);

#endif

