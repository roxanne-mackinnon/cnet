#ifndef READER_H
#define READER_H


#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

struct IdxFile {
  FILE * source;
  int startingByte;
  int count;
  int width;
  int height;
  int size;
};

typedef struct IdxFile IdxFile;


IdxFile * initFile(char * fileName);

void freeFile(IdxFile * file);

// assumes that source is open already
unsigned char * getMatrix(IdxFile * file, int n);

// convert a char matrix to a matrix of floats between 0 and 1.
float * convertMatrix(unsigned char * matrix, int size);

// print a matrix to stdout (good for seeing digits)
void printMatrix(IdxFile * file, int n);

#endif
