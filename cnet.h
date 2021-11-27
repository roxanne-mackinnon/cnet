/*
 * cnet.h - Lightweight library for training neural nets
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

#ifndef __CNET_H__
#define __CNET_H__

#include <math.h>
#include <stdlib.h>
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
