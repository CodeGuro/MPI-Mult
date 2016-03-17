all: prog2

prog2: prog2.o
	mpicc prog2.o -o prog2

prog2.o: prog2.c
	mpicc --std=c99 -c prog2.c -o prog2.o

clean:
	rm -f *.o prog2