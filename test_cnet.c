/*
 * test_cnet.c - Unit tests for cnet neural nets
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
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cnet.h"
#include "vector.h"

void testVectorGetSet(int height, int width, int seed);

void testVectorRandomize(int height, int width, int seed);

void testVectorSetElements(int height, int width, int seed);

void testVectorEqual();
  
void testVectorBinary(int oneHeight, int oneWidth, int twoHeight, int twoWidth);

void testVectorApply(int height, int width, float (*foo)(float));

void testNetInit(int depth, int maxSize);

void testNetRandomizeWeights(int * sizes, int seed);



int main(int argc, char * argv[]) {
  fprintf(stderr, "not yet implemented!\n");
  return 0;
}


void testNetInit(int depth, int maxSize) {

  int * sizes = malloc(depth * sizeof(int));
  int lastSize = maxSize;
  for (int i = 0; i < depth; i++) {
    sizes[i] = (rand() % lastSize) + 1;
    lastSize = sizes[i];
  }
  
  NeuralNet * net = initNet(depth, sizes);

  // Make sure of the simple stuff
  assert(net->depth == depth);
  assert(!memcmp(sizes, net->sizes, depth * sizeof(int)));

  for (int layer = 0; layer < net->depth - 1; layer++) {
    assert(net->neurons[layer]->height == sizes[layer]);
    assert(net->neurons[layer]->width == 1);

    assert(net->weights[layer]->height == sizes[layer + 1]);
    assert(net->weights[layer]->width == sizes[layer]);
  }

  assert(net->neurons[net->depth - 1]->height == sizes[depth - 1]);
  assert(net->neurons[net->depth - 1]->width == 1);  
  
  netFree(net);
  free(sizes);
}

void testVectorEqual(int doEqual, int seed) {
  int oneHeight, oneWidth, twoHeight, twoWidth;

  srand(seed);
  oneHeight = rand() % 10000;
  oneWidth = rand() % 10000;
  if (doEqual) {
    twoHeight = oneHeight;
    twoWidth = oneWidth;
  } else {
    twoHeight = rand() % 10000;
    twoWidth = rand() % 10000;
  }

  Vector * one = initVector(oneHeight, oneWidth);
  Vector * two = initVector(twoHeight, twoWidth);
  
  srand(seed);
  vectorRandomize(one);

  if (doEqual) {
    srand(seed);
  }

  vectorRandomize(two);

  assert(vectorEqual(one, two) == doEqual);

  vectorFree(one);
  vectorFree(two);
}


void testVectorGetSet(int height, int width, int seed) {
  // Should not get segfaults
  Vector * vec = initVector(height, width);
  srand(seed);
  // Set elements
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      vectorSet(vec, i, j, rand());
    }
  }
  srand(seed);
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      assert(vectorGet(vec, i, j) == (float) rand());
    }
  }
  vectorFree(vec);
}


void testVectorRandomize(int height, int width, int seed) {
  Vector * vec = initVector(height, width);
  srand(seed);
  vectorRandomize(vec);
  srand(seed);
  for (int i = 0; i < vec->height; i++) {
    for (int j = 0; j < vec->width; j++) {
      assert(vectorGet(vec, i, j) == ((rand() - (RAND_MAX / 2)) / (float) RAND_MAX));
    }
  }
  vectorFree(vec);
}

void testVectorSetElements(int height, int width, int seed) {
  float * region = malloc(height * width * sizeof(float));
  srand(seed);
  for (int i = 0; i < height * width; i++) {
    region[i] = (float) rand();
  }
  Vector * vec = initVector(height, width);
  vectorSetElements(vec, region);
  srand(seed);
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      assert(vectorGet(vec, i, j) == rand());
    }
  }
  free(region);
  vectorFree(vec);
}


