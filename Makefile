CC=gcc -g -Wall

idxreader.o: idxreader.c idxreader.h
	$(CC) -c idxreader.c

test_cnet: cnet.o vector.o test_cnet.c
	$(CC) -o test_cnet test_cnet.c cnet.o vector.o

cnet.o: cnet.c cnet.h vector.o
	$(CC) -c cnet.c

vector.o: vector.c vector.h
	$(CC) -c vector.c

clean:
	rm -rf *.o test_cnet
