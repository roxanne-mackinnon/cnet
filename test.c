#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "neural_net.h"
#include "vector.h"

void testVectorGetSet(int height, int width, int seed);

void testVectorRandomize(int height, int width, int seed);

void testVectorSetElements(int height, int width, int seed);

void testVectorEqual();
  
void testVectorBinary(int oneHeight, int oneWidth, int twoHeight, int twoWidth);

void testVectorApply(int height, int width, float (*foo)(float));

int main(int argc, char * argv[]) {

  // parameters: height, width, and random_seed
  testVectorRandomize(1, 1, 69);
  testVectorRandomize(200, 200, 0);
  testVectorRandomize(5000, 2, 10000);

  /* testVectorEqual(1, 69); */
  /* testVectorEqual(1, 4000); */
  /* testVectorEqual(0, 78765); */
  /* testVectorEqual(0, 987098); */

  return 0;
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


