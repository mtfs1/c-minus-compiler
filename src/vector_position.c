#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector_position.h"


struct VectorPos new_vec_position() {
  struct VectorPos vec = {0};
  vec.vec = (struct Position *)calloc(10, sizeof(struct Position));
  vec.capacity = 10;
  return vec;
}

void append_vec_position(struct VectorPos *vec, int line_num, int char_num) {
  if(vec->capacity == vec->size) {
    struct Position *temp = vec->vec;
    vec->capacity *= 2;
    vec->vec =
      (struct Position *)calloc(2 * vec->capacity, sizeof(struct Position));
    memcpy(vec->vec, temp, vec->size * sizeof(struct Position));
  }
  vec->vec[vec->size].line_num = line_num;
  vec->vec[vec->size].char_num = char_num;
  vec->size++;
}

