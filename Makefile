CC = gcc
TARGET = compiler
BISON_SRC = $(wildcard src/*.y)
SRC = $(wildcard src/*.c) $(BISON_SRC:.y=.c)
OBJ = $(SRC:.c=.o)
FLAGS = -g

all: $(OBJ)
	$(CC) -o $(TARGET) $(FLAGS) $^

%.o: %.c
	$(CC) -c -o $@ $(FLAGS) $<

%.c: %.y
	bison -o $@ $<

clean:
	rm src/*.o
	rm ./compiler

