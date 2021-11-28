/*
 * idxreader.c - Routines for parsing .idx data files
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
#include <endian.h>
#include <errno.h>
#include "idxreader.h"


/**
 * return 0 if all is well, -1 if header is malformed
 * assumes fewer than IDX_MAX_DIMS 
 */
static int initHeader(FILE * file, IdxFileHeader * header) {

  fseek(file, 0, SEEK_SET);

  // read in two blanks, a magic number, and the num of dimensions
  fread((void *)&result, 1, 4, file);

  // should start with two blanks
  if (result->blank[0] != 0x0 || result->blank[1] != 0x0) {
    errno = EINVAL;
    return -1;
  }

  // this relies on the implementation of IdxDataType!! It should
  // still be fine as long as IdxDataType is consistent with idx
  // standard.
  if (result->magic_number < IDX_UCHAR || result->magic_number > IDX_DOUBLE) {
    errno = EINVAL;
    return -1;
  }

  // is this safe for unsigned int arr[]?
  memset(header->dims, 0, IDX_MAX_DIMS * sizeof(unsigned int));
  
  // try to read in the dimensions, watch out for short files
  int ret = fread(header->dims, sizeof(unsigned int), header->num_dims, file);
  if (ret != header->num_dims) {
    return -1;
  }
  
  // convert from big endian format to little endian (big endian is standard for idx)
  for (int i = 0; i < header->num_dims; i++) {
    result->dims[i] = be32toh(result->dims[i]);
  }
  
  return 0;
}

IdxFile * initFile(char * fileName) {
  FILE * f = fopen(fileName, "r");
  // if any occurs, return and pass whatever errno was set by fopen
  if (f == NULL) {
    return NULL;
  }

  IdxFile * result = malloc(sizeof(IdxFile));

  int ret = initHeader(f, &(result->header));
  result->source = f;
  result->startingByte = 4 + 4 * dims;
  result->count = count;
  result->width = width;
  result->height = height;
  result->size = width * height;

  return result;
}
void freeFile(IdxFile * file) {

  if (file == NULL) {
    return;
  }
  
  fclose(file->source);
  free(file);
}

void printMatrix(IdxFile * file, int n) {

  if (file == NULL) {
    return;
  }
  
  unsigned char * matrix = getMatrix(file, n);
  int width = file->width;
  int height = file->height;
  // iterate through each row and column of the image
  // print 3 diff chars for 3 brightness levels
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      switch (matrix[i * width + j] / 100) {
      case (0): {putchar(' '); break;}
      case (1):	{putchar('-'); break;}
      case (2):	{putchar('&'); break;}
      }
    }
    // newline after each row
    putchar('\n');
  }
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
1
  unsigned char * result = malloc((file->size) * sizeof(unsigned char));
  for (int i = 0; i < file->size; i++) {
    result[i] = fgetc(file->source);
  }

  return result;
}

typedef struct IdxFileHeader {
  char blank[2];
  char magic_number; /* encodes the type/size of the data */
  unsigned char num_dims; /* number of dimensions of data, normally 1-2 */
  unsigned int * dims; /* vector of dimension sizes */
} IdxFileHeader;

