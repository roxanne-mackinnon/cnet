/*
 * cnet.c - Lightweight library for training neural nets
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
#include "cnet.h"

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

float softMax(float f) {
  // change this in the assembly code
  if (f <= 0.0) {
    return 0.0;
  }
  return f;
}


// depth = number of node layers including output and input
// sizes = height of each node layer
// weights = array of weight matrices
// neurons = array of node layers




// Allocate space for a new neural net.
// 'depth' is the number of neuron layers in the net.
// 'sizes' is an array representing the height of each neuron layer.
NeuralNet * initNet(int depth, int * sizes) {
  // The net we will return.
  NeuralNet * result = malloc(sizeof(NeuralNet));

  // The number of neuron layers.
  result->depth = depth;

  // Allocate space for the array of layer heights, array of neuron
  // layers, and array of weight matrices.
  result->sizes = malloc(depth * sizeof(int));
  result->neurons = malloc(depth * sizeof(Vector *));
  result->weights = malloc((depth - 1) * sizeof(Vector *));

  // Copy in the layer sizes
  memcpy(result->sizes, sizes, depth * sizeof(int));

  // For each layer, create the actual neuron vector and weight matrix
  // corresponding to that layer, such that multiplying the neuron
  // vector by the matrix will yield the output vector for that layer.
  int layer;
  for (layer = 0; layer < depth - 1; layer++) {
    // The neuron vector is a column vector, so it has width of 1.
    result->neurons[layer] = initVector(sizes[layer], 1);
    
    // Each weight matrix has the height of the neuron vector coming
    // after it, and the width of the neuron vector before it.
    result->weights[layer] = initVector(sizes[layer + 1], sizes[layer]);
  }

  // Allocate space for the very last output vector for the net.
  result->neurons[depth - 1] = initVector(sizes[depth - 1], 1);
  
  return result;
}

void netFree(NeuralNet * net) {

  for (int layer = 0; layer < net->depth - 1; layer++) {
    vectorFree(net->neurons[layer]);
    vectorFree(net->weights[layer]);
  }
  vectorFree(net->neurons[net->depth - 1]);
  
  free(net->sizes);
  free(net->weights);
  free(net->neurons);
  
  free(net);
}

// Give the net a random set of weights, lying between -1 and 1.
// Does not alter the weight's locations in memory.
void randomizeWeights(NeuralNet * net) {
  // There is one less matrix of weights than vector of neurons.
  // For each matrix of weights, randomize the matrix.
  for (int layer = 0; layer < net->depth - 1; layer++) {
    vectorRandomize(net->weights[layer]);
  }
}

// Return the value of the neuron at the given layer, at the given
// row. Rows are counted from 0, from the top.
float netGetNeuron(NeuralNet * net, int layer, int row) {
  return vectorGet(net->neurons[layer], row, 0);
}

// Return the value of the weight coming directly after the given
// neuron layer, connecting the given neuron in the input row to the
// given neuron in the output row.
float netGetWeight(NeuralNet * net, int layer, int outputRow, int inputRow) {
  return vectorGet(net->weights[layer], outputRow, inputRow);
}

// Set the value of the neuron in the given layer, at the given
// row. Indices start at 0, counting from the top.
void netSetNeuron(NeuralNet * net, int layer, int row, float value) {
  vectorSet(net->neurons[layer], row, 0, value);
}

// Set the value of the weight in the given layer, connecting the
// given input neuron to the given output neuron.
void netSetWeight(NeuralNet * net, int layer, int outputRow, int inputRow, float value) {
  vectorSet(net->weights[layer], outputRow, inputRow, value);
}

// Take an input vector and populate the layers of the neural net
// using matrix multiplication and the softMax function.
void netCalculate(NeuralNet * net, Vector * input) {
  
  // If the input vector is of invalid size, print an error message and return.
  if ((input->height != net->sizes[0]) || (input->width != 0)) {
    printf("Error: input vector of dimension %d x %d"
	   "invalid for net of input dimension %d x %d.\n",
	   input->height, input->width, net->sizes[0], net->sizes[1]);
    return;
  }
  
  // First argument is the destination, second argument is the source.
  vectorCopy(net->neurons[0], input);

  Vector * intermediate;
  for (int layer = 1; layer < net->depth; layer++) {
    intermediate = vectorMultiply(net->weights[layer - 1], net->neurons[layer - 1]);
    vectorApply(intermediate, softMax);
    vectorCopy(net->neurons[layer], intermediate);
    vectorFree(intermediate);
  }
  return;
}


float nodeDerivative(NeuralNet * net, int nodeLayer, int nodeRow, int weightLayer, int outputRow, int inputRow) {
  float nodeValue = netGetNeuron(net, nodeLayer, nodeRow);
  if (nodeValue <= 0.0) {
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
  //
  // lets try it the slow way first

  if (weightLayer + 1 == nodeLayer) {
    if (nodeRow != outputRow) {
      return 0.0;
    }
    else {
      return netGetNeuron(net, weightLayer, inputRow);
    }
  }

  // the node is further to the left and our value is not 0
  float result = 0;
  float value;
  int inputHeight = net->sizes[nodeLayer - 1];
  // add together the weights times the derivatives for the previous layer
  for (int i = 0; i < inputHeight; i++) {
    value = nodeDerivative(net, nodeLayer - 1, i, weightLayer, outputRow, inputRow);
    result += netGetWeight(net, nodeLayer - 1, nodeRow, i) * value;
  }
  return result;
}

