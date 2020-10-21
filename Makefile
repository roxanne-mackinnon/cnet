
test: net vector test.c
	gcc -o test test.c neural_net.o vector.o && ./test

net: neural_net.c neural_net.h vector
	gcc -g -c neural_net.c vector.o

vector: vector.c vector.h
	gcc -g -c vector.c

clean:
	rm -rf *.o *.dSYM *.s
	rm -f test
