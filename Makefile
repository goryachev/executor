PROJECT_NAME=executor
CC=g++
CFLAGS=-std=c11 -O3 -g -Wall -pthread -I.
DEPS = executor.h
OBJ = main.o executor.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(PROJECT_NAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o
	rm $(PROJECT_NAME)
