#include "vector.h"

Vector * initVector(int height, int width) {
  Vector * result = calloc(1, sizeof(Vector));
  result->elements = calloc(height * width, sizeof(float));
  result->height = height;
  result->width = width;
  return result;
}

float vectorGet(Vector * v, int row, int column) {
  return *(v->elements + row * v->width + column);
}

void vectorSet(Vector * v, int row, int column, float element) {
  *(v->elements + row * v->width + column) = element;
}


void vectorFree(Vector * v) {
  if (v == NULL) {
    return;
  }

  free(v->elements);
  free(v);
  return;
}

static float randomFloat(FILE * source) {
  char c = fgetc(source);
  return ((float) c) / 128.0;
}


void vectorRandomize(Vector * v) {
  FILE * randomSource = fopen("/dev/random", "r");
  for (int row = 0; row < v->height; row++) {
    for (int column = 0; column < v->width; column++) {
      vectorSet(v, row, column, randomFloat(randomSource));
    }
  }
  fclose(randomSource);
}





Vector * vectorCopy(Vector * v) {
  Vector * result = initVector(v->height, v->width);
  for (int i = 0; i < v->height; i++) {
    for (int j = 0; j < v->width; j++) {
      vectorSet(result, i, j, vectorGet(v, i, j));
    }
  }
  return result;
}

// width of a must equal height of b
Vector * vectorMultiply(Vector * a, Vector * b) {
  Vector * result = initVector(a->height, b->width);
  float sum;
  for (int a_row = 0; a_row < a->height; a_row++) {
    for (int b_column = 0; b_column < a->width; b_column++) {
      sum = 0;
      for (int row_index = 0; row_index < a->width; row_index++) {
	sum += vectorGet(a, a_row, row_index) * vectorGet(b, row_index, b_column);
      }
      vectorSet(result, a_row, b_column, sum);
    }
  }
  return result;
}

// a must have same dimensions as b
Vector * vectorAdd(Vector * a, Vector * b) {
  Vector * result = initVector(a->height, a->width);
  for (int row = 0; row < a->height; row++) {
    for (int column = 0; column < a->width; column++) {
      vectorSet(result, row, column, vectorGet(a, row, column) + vectorGet(b, row, column));
    }
  }
  return result;
}

// a must have same dimensions as b
Vector * vectorSubstract(Vector * a, Vector * b) {
  Vector * result = initVector(a->height, a->width);
  for (int row = 0; row < a->height; row++) {
    for (int column = 0; column < a->width; column++) {
      vectorSet(result, row, column, vectorGet(a, row, column) - vectorGet(b, row, column));
    }
  }
  return result;
}

// scale all elements of a by a factor of f
Vector * vectorScale(Vector * a, float factor) {
  Vector * result = initVector(a->height, a->width);
  for (int row = 0; row < a->height; row++) {
    for (int column = 0; column < a->width; column++) {
      vectorSet(result, row, column, factor * vectorGet(a, row, column));
    }
  }
  return result;
}


// apply foo to all elements of a
Vector * vectorApply(Vector * a, float (*foo)(float)) {
  Vector * result = initVector(a->height, a->width);
  for (int row = 0; row < a->height; row++) {
    for (int column = 0; column < a->width; column++) {
      vectorSet(result, row, column, foo(vectorGet(a, row, column)));
    }
  }
  return result;
}
