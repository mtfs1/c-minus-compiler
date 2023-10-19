CC = gcc
TARGET = compiler
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

all: $(OBJ)
	$(CC) -o $(TARGET) $^

%.o: %.c
	$(CC) -c -o $@ $<

clean:
	rm src/*.o
	rm ./compiler

