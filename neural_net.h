#ifndef NEURAL_H
#define NEURAL_H

// may not actually need this include
#include <math.h>
// used for /dev/random, a source for randomness
#include <stdio.h>
// for vector manipulation
#include "vector.h"

struct NeuralNet {
  int layerCount;
  int * layerSizes;
  Vector ** weightLayers;
  Vector ** activationLayers;
};

typedef struct NeuralNet NeuralNet;


static float softMax(float f);

static float randomFloat(FILE * source);

NeuralNet * initNet(int activationLayerCount, int * layerSizes);

void randomizeWeights(NeuralNet * net);

float netGetNodeValue(NeuralNet * net, int nodeLayer, int nodeRow);

float netGetWeightValue(NeuralNet * net, int weightLayer, int weightRow, int weightColumn);

void netSetWeightValue(NeuralNet * net, int weightLayer, int weightRow, int weightColumn, float value);

void netSetNodeValue(NeuralNet * net, int nodeLayer, int nodeRow, float value);

float nodeDerivative(NeuralNet * net, int nodeLayer, int nodeRow, int weightLayer, int weightRow, int weightColumn);

// populate the neural net for the given input
void netCalculate(NeuralNet * net, Vector * input);

#endif
