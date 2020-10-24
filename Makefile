
idxreader: idxreader.c idxreader.h
	gcc -g -c idxreader.c

test: net vector test.c
	gcc -g -o test test.c neural_net.o vector.o

net: neural_net.c neural_net.h vector
	gcc -g -c neural_net.c

vector: vector.c vector.h
	gcc -g -c vector.c

clean:
	rm -rf *.o *.dSYM *.s
	rm -f test
