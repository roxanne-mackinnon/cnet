/*
 * idxreader.h - Routines for parsing .idx data files
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
#ifndef __IDX_READER_H__
#define __IDX_READER_H__

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>


// We arbitrarily decide that an idx file may have no more than 64 dimensions
// (noting that 3 is the standard number of dimensions for any normal ML dataset)
#define IDX_MAX_DIMS 64

typedef struct IdxFileHeader {
  char blank[2];
  char magic_number; /* encodes the type/size of the data */
  unsigned char num_dims; /* number of dimensions of data, normally 1-3 */
  unsigned int dims[IDX_MAX_DIMS]; /* vector of dimension sizes */
} IdxFileHeader;

typedef struct IdxFile {
  FILE * source;
  IdxFileHeader header;
  int startingByte;
  int count;
  int width;
  int height;
  int size;
} IdxFile;

typedef enum IdxDataType {
  IDX_UCHAR  = 0x08,
  IDX_SCHAR  = 0x09,
  IDX_SHORT  = 0x0B,
  IDX_INT    = 0x0C,
  IDX_FLOAT  = 0x0D,
  IDX_DOUBLE = 0x0E
} IdxDataType;


IdxFile * initFile(char * fileName);

void freeFile(IdxFile * file);

// assumes that source is open already
unsigned char * getMatrix(IdxFile * file, int n);

// convert a char matrix to a matrix of floats between 0 and 1.
float * convertMatrix(unsigned char * matrix, int size);

// print a matrix to stdout (good for seeing digits)
void printMatrix(IdxFile * file, int n);

#endif
