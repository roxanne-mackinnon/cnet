#include <stdio.h>
#include <stdlib.h>
#include "neural_net.h"
#include "vector.h"



int main(int argc, char * argv[]) {

  int * layerSizes = calloc(argc - 1, sizeof(int));
  for (int i = 0; i < argc - 1; i++) {
    layerSizes[i] = atoi(argv[i + 1]);
  }

  NeuralNet * net = initNet(argc - 1, layerSizes);
  randomizeWeights(net);


  // the vectors should already be initialized

  Vector * input1;
  Vector * input2;
  Vector * input3;
  
  input1 = initVector(layerSizes[0], 1);
  input2 = initVector(layerSizes[0], 1);
  input3 = initVector(layerSizes[0], 1);

  vectorRandomize(input1);
  vectorRandomize(input2);


  /* vectorPrint(input1); */
  /* printf("---\n"); */
  /* vectorPrint(input2); */
  /* printf("---\n"); */

  // lets just see for right now if this causes any problems
  char c = getchar();
  netCalculate(net, input1);
  netCalculate(net, input2);
  return 0;
}
