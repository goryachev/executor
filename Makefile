PROJECT_NAME=threadPool
CC=g++
CFLAGS=-std=c11 -O3 -g -Wall -pthread -I.
DEPS = function_pool.h executor.h
OBJ = main.o executor.o function_pool.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(PROJECT_NAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o
	rm $(PROJECT_NAME)
