CC = gcc
TARGET = compiler
BISON_SRC = $(wildcard src/*.y)
SRC = $(wildcard src/*.c) $(BISON_SRC:.y=.c)
OBJ = $(SRC:.c=.o)

all: $(OBJ)
	$(CC) -o $(TARGET) $^

%.o: %.c
	$(CC) -c -o $@ $<

%.c: %.y
	bison -o $@ $<

clean:
	rm src/*.o
	rm ./compiler

