#include "vector.h"
#include <math.h>
#include <stdio.h>
#include "neural_net.h"
// f(value) = max(0, value)
// d/dx(f) = (x > 0) * d\dw(x)
// Cost = sum over last layer (expected - actual)^2
// d/dweight[layer, row, column] (cost) = sum over last layer 2(expected - actual) * d\dweight[layer, row column](actual)
// actual = max(0, sum over previous layer * vector of weights)
// d/dw(actual) = ((sum over previous layer * vector of weights) > 0) * d\dw(sum over previous layer * vector of weights)
// = (prev layer lc > 0) * d\dw (prev layer lc)
// for each node value, if the value is greater than 0, the derivative of that node is just
// the derivative of the linear combination making up the node.


// we need to compute the derivative of each node with all the weights before it.
// using softmax, the derivative of a node with all the weights before it 0 if the node's value is 0.
// if the nodes value is greater than 0, the weights are the derivatives of (prev value * weight)

static float softMax(float f) {
  return (f > 0) ? f : 0;
}

static float randomFloat(FILE * source) {
  char c = fgetc(source);
  return ((float) c) / 128.0;
}

// allocate space for a net and for the layers and weight matrices
NeuralNet * initNet(int activationLayerCount, int * layerSizes) {
  NeuralNet * result = calloc(1, sizeof(NeuralNet));
  result->layerCount = activationLayerCount;
  result->layerSizes = layerSizes;
  result->weightLayers = calloc(activationLayerCount - 1, sizeof(Vector *));
  result->activationLayers = calloc(activationLayerCount, sizeof(Vector *));
  return result;
}

void randomizeWeights(NeuralNet * net) {
  FILE * randSource = fopen("/dev/urandom", "r");
  for (int layer = 0; layer < net->layerCount - 1; layer++) {
    for (int row = 0; row < net->layerSizes[layer + 1]; row++) {
      for (int column = 0; column < net->layerSizes[layer]; column++) {
	netSetWeightValue(net, layer, row, column, randomFloat(randSource));
      }
    }
  }
  fclose(randSource);
}


float netGetNodeValue(NeuralNet * net, int nodeLayer, int nodeRow) {
  return vectorGet(net->activationLayers[nodeLayer], nodeRow, 1);
}

float netGetWeightValue(NeuralNet * net, int weightLayer, int weightRow, int weightColumn) {
  return vectorGet(net->weightLayers[weightLayer], weightRow, weightColumn);
}

void netSetNodeValue(NeuralNet * net, int nodeLayer, int nodeRow, float value) {
  vectorSet(net->activationLayers[nodeLayer], nodeRow, 1, value);
}

void netSetWeightValue(NeuralNet * net, int weightLayer, int weightRow, int weightColumn, float value) {
  vectorSet(net->weightLayers[weightLayer], weightRow, weightColumn, value);
}

void netCalculate(NeuralNet * net, Vector * input) {
  // when this function exits, it should have no additional memory side effects
  // NO! even if the input vector is modified from outside, we should be okay with that
  // we might have some sort of reassignment deal going on
  // i dont think this should cause a segfault.
  // we are telling the computer to free that vector *,
  // but then we put another vector * in its place.
  vectorFree(net->activationLayers[0]);
  net->activationLayers[0] = vectorCopy(input);
  Vector * multiple;
  Vector * application;
  for (int i = 1; i < net->layerCount; i++) {

    multiple = vectorMultiply(net->weightLayers[i-1], net->activationLayers[i-1]);
    application = vectorApply(multiple, softMax);
    vectorFree(multiple);
    vectorFree(net->activationLayers[i]);
    net->activationLayers[i] = vectorCopy(application);
    vectorFree(application);
  }
  // hopefully free from memory leaks
  // test:
  //   have a small set of vectors.
  //   if we can repeatedly call netCalculate on different members of this set,
  //   without having memory leaks, we will be good.
}


float nodeDerivative(NeuralNet * net, int nodeLayer, int nodeRow, int weightLayer, int weightRow, int weightColumn) {
  float nodeValue = netGetNodeValue(net, nodeLayer, nodeRow);
  if (nodeValue <= 0) {
    return 0;
  }

  // otherwise, return the derivative of the sum
  // if the weight is not directly adjacent,
  // the sum is just the weight times the derivative of the node
  //
  // if the weight is directly adjacent,
  // and the node is not connected to that weight,
  // the derivative is 0.
  // if the weight is directly adjacent and the node IS connected,
  // the derivative is the node value 'behind' the weight


  if (weightLayer == nodeLayer - 1) {
    if (nodeRow != weightRow) {
      return 0;
    }
    else {
      return netGetNodeValue(net, weightLayer, weightColumn);
    }
  }

  // the node is further to the left and our value is not 0
  float result = 0;
  int prevLayerHeight = net->activationLayers[nodeRow - 1]->height;
  // add together the weights times the derivatives for the previous layer
  for (int i = 0; i < prevLayerHeight; i++) {
    result += netGetWeightValue(net, nodeLayer - 1, nodeRow, i) * nodeDerivative(net, nodeLayer - 1, i, weightLayer, weightRow, weightColumn);
  }
  return result;
}
