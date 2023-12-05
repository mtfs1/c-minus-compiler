#include <stdlib.h>

#ifndef VECTOR_PARAMS
#define VECTOR_PARAMS

struct VectorParams {
  int size;
  int capacity;
  int *vec;
};

struct VectorParams new_vec_params();
void append_vec_params(struct VectorParams *vec, int type);

#endif

