/*
 * vector.h - C library for simple vector operations
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
