CC = mpicc
CFLAGS = --std=c99
DEPS = matrix.h
OBJ = matrix.o prog2.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

prog2: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o prog2