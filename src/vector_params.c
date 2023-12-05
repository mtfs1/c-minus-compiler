#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector_params.h"


struct VectorParams new_vec_params() {
  struct VectorParams vec = {0};
  vec.vec = (int *)calloc(10, sizeof(int));
  vec.capacity = 10;
  return vec;
}

void append_vec_params(struct VectorParams *vec, int type) {
  if(vec->capacity == vec->size) {
    int *temp = vec->vec;
    vec->capacity *= 2;
    vec->vec = (int *)calloc(2 * vec->capacity, sizeof(int));
    memcpy(vec->vec, temp, vec->size * sizeof(int));
  }
  vec->vec[vec->size] = type;
  vec->size++;
}

