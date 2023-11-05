BIN := main

SRC := $(wildcard src/impl/*.c) 

OBJECTS := $(SRC:.c=.o)
OBJECTS := $(patsubst src/impl/%, %, $(OBJECTS))
OBJECTS := $(addprefix obj/, $(OBJECTS))

CC := gcc
CFLAGS := -Wall -g # -lpthread
LDFLAGS := -lpanel -lncurses

.PHONY: all clean

all: $(BIN)

obj/%.o: src/impl/%.c
	$(CC) -c $< $(CFLAGS)  -o $(patsubst src/impl/%, %, $@)

obj/main.o: src/main.c
	$(CC) $(CFLAGS)  -c src/main.c -o obj/main.o

$(BIN): $(OBJECTS) obj/main.o
	$(CC) $(CFLAGS)  $(OBJECTS) obj/main.o $(LDFLAGS) -o bin/$(BIN)

clean:
	rm -rf main obj/*.o