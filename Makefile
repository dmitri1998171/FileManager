BIN_1 := main 
SOURCES_1 := src/main.c
DEPENDENCIES = 
CC := gcc
CFLAGS := -g
LDFLAGS := -lpthread -lncurses

$(BIN_1): $(SOURCES_1) $(DEPENDENCIES)
	clear && mkdir -p bin && $(CC) $(SOURCES_1) $(DEPENDENCIES) $(CFLAGS) $(LDFLAGS) -o bin/$(BIN_1)
depend:
	sudo apt-get install libncurses5-dev libncursesw5-dev
clean:
	rm -rf bin