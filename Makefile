CC=g++
CFLAGS=-std=c11 -O3 -g -Wall -pthread -I.
DEPS = function_pool.h
OBJ = main.o function_pool.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

threadPool: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o
	rm threadPool
