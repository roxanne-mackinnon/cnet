#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <stdio.h>

struct Vector {
  int height;
  int width;
  float * elements;
};

typedef struct Vector Vector;
// dont feel like doing index chasing. so we'll start at 1

static float randomFloat(FILE * source);

Vector * initVector(int height, int width);

float vectorGet(Vector * v, int row, int column);

void vectorSet(Vector * v, int row, int column, float element);

void vectorFree(Vector * v);

void vectorRandomize(Vector * v);

Vector * vectorCopy(Vector * v); 

// width of a must equal height of b
Vector * vectorMultiply(Vector * a, Vector * b);

// a must have same dimensions as b
Vector * vectorAdd(Vector * a, Vector * b);

// a must have same dimensions as b
Vector * vectorSubstract(Vector * a, Vector * b);

// scale all elements of a by a factor of f
Vector * vectorScale(Vector * a, float factor);

// apply foo to all elements of a
Vector * vectorApply(Vector * a, float (*foo)(float));

#endif
