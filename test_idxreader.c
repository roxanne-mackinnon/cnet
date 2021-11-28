#include <stdio.h>
#include <stdlib.h>
#include "idxreader.h"



void test_one_init_file
(char * filename, IdxDataType type,
 int num_dims, int * dims) {
  IdxFile * file = initFile(filename);
  assert(file != NULL);
  IdxFileHeader head = file->header;
  assert(head.magic_number == type);
  assert(head.num_dims == num_dims);
  for (int i = 0; i < head.num_dims; i++) {
    assert(head.dims[i] == dims[i]);
  }
  freeFile(file);
}

/**
 * Test that file headers are being parsed correctly  
 */
void test_init_file(void) {
  static char filenames[3][] =
    {"test1.idx",
     "test2.idx",
     "test3.idx"};
  // first file should have:
  // datatype 'uchar'
  // num_dims 2
  // dims = {3, 2, 2}
  test_one_init_file(filenames[0], IDX_UCHAR, 2, {3,2});

  // second file should have:
  // datatype double
  // num_dims 1
  // dims = 5
  test_one_init_file(filenames[1], IDX_DOUBLE, 1, {5});

  // third file should have:
  // datatype int
  // num_dims 3
  // dims = {1,1,1}
  test_one_init_file(filenames[2], IDX_INT, 3, {1,1,1});
}
