#include "idxreader.h"


void freeFile(IdxFile * file) {
  fclose(file->source);
  free(file);
}

void printMatrix(IdxFile * file, int n) {
  unsigned char * matrix = getMatrix(file, n);
  int width = file->width;
  for (int i = 0; i < file->height; i++) {
    for (int j = 0; j < file->width; j++) {
      switch (matrix[i * width + j] / 100) {
      case (0): {putchar(' '); break;}
      case (1):	{putchar('-'); break;}
      case (2):	{putchar('&'); break;}
      }
    }
    putchar('\n');
  }
  printf("---\n");
}

float * convertMatrix(unsigned char * matrix, int size) {
  float * result = malloc(size * sizeof(float));
  for (int i = 0; i < size; i++) {
    result[i] = (matrix[i] - 127) / 127.0;
  }
  return result;
}

unsigned char * getMatrix(IdxFile * file, int n) {
  int test = fseek(file->source, n*(file->size) + file->startingByte, 0);
  if (test == -1) {
    printf("Request for matrix %d out of bounds for given idx file of count %d.\n", n, file->count);
    exit(1);
  }

  unsigned char * result = malloc((file->size) * sizeof(unsigned char));
  for (int i = 0; i < file->size; i++) {
    result[i] = fgetc(file->source);
  }

  return result;
}

IdxFile * initFile(char * fileName) {
  FILE * f = fopen(fileName, "r");
  if (f == NULL) {
    printf("Error: invalid file name %s\n.", fileName);
    exit(1);
  }

  IdxFile * result = malloc(sizeof(IdxFile));

  // Throw away the first 3 parts of the header
  getc(f);
  getc(f);
  getc(f);
  
  unsigned char dims = getc(f);

  getc(f);
  getc(f);
  unsigned int count = getc(f);
  count = (count<<8) + getc(f);

  // Get the number of matrices
  unsigned int height;
  unsigned int width;

  if (dims == 1) {
    height = 1;
    width = 1;
  }
  else if (dims == 3) {
    getc(f);
    getc(f);
    height = getc(f);
    height = (height<<8) + getc(f);

    getc(f);
    getc(f);
    width = getc(f);
    width = (width<<8) + getc(f);
  }
  else {
    printf("Invalid idx file. Probably you are reading it wrong.\n");
    exit(1);
  }

  result->source = f;
  result->startingByte = 4 + 4 * dims;
  result->count = count;
  result->width = width;
  result->height = height;
  result->size = width * height;

  return result;
}
