
all : test.c neural_net.o vector.o
	gcc -o test test.c neural_net.o vector.o

test : test.c neural_net.c vector.c
	gcc -g -c vector.c
	gcc -g -c neural_net.c
	gcc -g -o test test.c neural_net.o vector.o

neural : neural_net.c neural_net.h
	gcc -c neural_net.c

vector : vector.c vector.h
	gcc -c vector.c

clean :
	rm -f neural_net.o vector.o test
	rm -rf ./test.dSYM
