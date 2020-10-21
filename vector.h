#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


struct Vector {
  int height;
  int width;
  float * elements;
};

typedef struct Vector Vector;
// dont feel like doing index chasing. so we'll start at 1

float randomFloat();

Vector * initVector(int height, int width);

float vectorGet(Vector * vec, int row, int column);

void vectorSet(Vector * vec, int row, int column, float element);

void vectorFree(Vector * vec);

void vectorRandomize(Vector * vec);

void vectorPrint(Vector * vec);

void vectorCopy(Vector * dest, Vector * source);

int vectorEqual(Vector * one, Vector * two);

void vectorSetElements(Vector * vec, float * data);

// width of a must equal height of b
Vector * vectorMultiply(Vector * left, Vector * right);

// a must have same dimensions as b
Vector * vectorAdd(Vector * left, Vector * right);

// a must have same dimensions as b
Vector * vectorSubstract(Vector * left, Vector * right);

// scale all elements of a by a factor of f
Vector * vectorScale(Vector * vec, float factor);

// apply foo to all elements of a
void vectorApply(Vector * vec, float (*map)(float));

#endif
