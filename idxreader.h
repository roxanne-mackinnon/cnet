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
