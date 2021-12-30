BIN := main

SRC := src/main.c
MODEL := src/model.c
CONTROLLER := src/controller.c
COPY := src/copy.c
VIEW := src/view.c

CC := gcc
CFLAGS := -Wall -lpthread -g
LDFLAGS := -lncurses

.PHONY: all clean

all: $(BIN)

obj/copy.o: $(COPY)
	clear && $(CC) $(COPY) -c -o obj/copy.o


obj/pbar.o: $(MODEL)
	clear && $(CC) $(MODEL) -c -o obj/model.o

obj/controller.o: $(CONTROLLER)
	clear && $(CC) $(CONTROLLER) -c -o obj/controller.o

obj/view.o: $(VIEW)
	clear && $(CC) $(VIEW) -c -o obj/view.o


obj/main.o: $(SRC)
	clear && $(CC) $(SRC) -c -o obj/main.o

$(BIN): obj/model.o obj/copy.o obj/controller.o obj/view.o obj/main.o
	clear && $(CC) obj/model.o obj/copy.o obj/controller.o obj/view.o obj/main.o $(LDFLAGS) $(CFLAGS) -o $(BIN) 

clean:
	rm -rf main pbar copy *.o