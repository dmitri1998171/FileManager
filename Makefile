BIN := main

SRC := src/main.c
PROGRESS_BAR := src/pbar.c
COPY := src/copy.c
VIEW := src/view.c
# EXT := $(PROGRESS_BAR) $(COPY) $(VIEW)

CC := gcc
CFLAGS := -Wall -lpthread
LDFLAGS := -lncurses

.PHONY: all clean

obj/pbar.o: $(PROGRESS_BAR)
	clear && $(CC) $(PROGRESS_BAR) -c -o obj/pbar.o

obj/copy.o: $(COPY)
	clear && $(CC) $(COPY) -c -o obj/copy.o

obj/view.o: $(VIEW)
	clear && $(CC) $(VIEW) -c -o obj/view.o

obj/main.o: $(SRC)
	clear && $(CC) $(SRC) -c -o obj/main.o

$(BIN): obj/pbar.o obj/copy.o obj/view.o obj/main.o
	clear && $(CC) obj/pbar.o obj/copy.o obj/view.o obj/main.o $(LDFLAGS) $(CFLAGS) -o $(BIN)  

all: $(BIN)

clean:
	rm -rf main pbar copy *.o