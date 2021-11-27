/*
 * vector.c - C library for simple vector operations
 * 
 * Copyright (C) 2021 Roxanne MacKinnon
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * <rdm3@williams.edu> Roxanne MacKinnon
 */
#include "vector.h"

// lets slightly change the vector implementation
Vector * initVector(int height, int width) {
  
  Vector * result = malloc(sizeof(Vector));
  result->elements = malloc(height * width * sizeof(float));
  
  result->height = height;
  result->width = width;
  
  return result;
}

float vectorGet(Vector * vec, int row, int column) {
  return vec->elements[row * (vec->width) + column];
}

void vectorSet(Vector * vec, int row, int column, float element) {
  vec->elements[row * (vec->width) + column] = element;
}

void vectorSetElements(Vector * vec, float * data) {
  memcpy(vec->elements, data, (vec->width) * (vec->height) * sizeof(float));
}

int vectorEqual(Vector * left, Vector * right) {
  if ((left->height != right->height) || (left->width != right->width)) {
    return 0;
  }

  int byteCount = (left->height) * (left->width) * sizeof(float);

  return !memcmp(left->elements, right->elements, byteCount);
}

void vectorFree(Vector * vec) {
  if (vec == NULL) {
    return;
  }

  free(vec->elements);
  free(vec);
  return;
}

float randomFloat() {
  return (rand() - (RAND_MAX / 2)) / (float)RAND_MAX;
}

// may cause a compiler error, as randomFloat doesn't take an argument.
void vectorRandomize(Vector * vec) {
  int count = (vec->height) * (vec->width);
  for (int i = 0; i < count; i++) {
    vec->elements[i] = randomFloat();
  }
}

void vectorPrint(Vector * vec) {
  for (int i = 0; i < vec->height; i++) {
    for (int j = 0; j < vec->width; j++) {
      printf("%-6.6f\t", vectorGet(vec, i, j));
    }
    putchar('\n');
  }
  putchar('\n');
}

// Assume that destination has the same dimensions as source.
void vectorCopy(Vector * destination, Vector * source) {
  memcpy(destination->elements, source->elements, (source->height) * (source->width) * sizeof(float));
}

// width of a must equal height of b
Vector * vectorMultiply(Vector * left, Vector * right) {
  Vector * result = initVector(left->height, right->width);
  float sum;
  for (int leftRow = 0; leftRow < left->height; leftRow++) {
    for (int rightCol = 0; rightCol < right->width; rightCol++) {
      sum = 0;
      for (int i = 0; i < left->width; i++) {
	sum += vectorGet(left, leftRow, i) * vectorGet(right, i, rightCol);
      }
      vectorSet(result, leftRow, rightCol, sum);
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
Vector * vectorSubtract(Vector * a, Vector * b) {
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


// This should be the only one that modifies the original vector
void vectorApply(Vector * vec, float (*foo)(float)) {
  int count = vec->height * vec->width;
  for (int i = 0; i < count; i++) {
    vec->elements[i] = foo(vec->elements[i]);
  }
}
