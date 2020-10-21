#ifndef NEURAL_H
#define NEURAL_H

// may not actually need this include
#include <math.h>
// for mallo and free and stuff
#include <stdlib.h>
// for vector manipulation
#include "vector.h"


struct NeuralNet {
  int depth;
  int * sizes;
  Vector ** weights;
  Vector ** neurons;
};

typedef struct NeuralNet NeuralNet;


float softMax(float f);

NeuralNet * initNet(int activationLayerCount, int * layerSizes);

void netFree(NeuralNet * net);

void randomizeWeights(NeuralNet * net);

void netCalculate(NeuralNet * net, Vector * input);

float netGetNeuron(NeuralNet * net, int layer, int row);

float netGetWeight(NeuralNet * net, int layer, int outputRow, int inputRow);

void netSetWeight(NeuralNet * net, int layer, int outputRow, int inputRow, float value);

void netSetNeuron(NeuralNet * net, int layer, int row, float value);

float nodeDerivative(NeuralNet * net, int nodeLayer, int nodeRow, int weightLayer, int outputRow, int inputRow);


#endif
