PROG_NAME := popChess
CC ?= gcc
SRC := ./src/main.c ./src/control.c ./src/display.c ./src/board.c
OBJ_DIR := ./obj
OBJ_FILES := ./obj/*.o
#CFLAGS := -Wall -Wextra -Werror -std=c99 -pedantic 
CFLAGS := -Wall -Wextra -std=c99 -pedantic \
          -DPROG_NAME=\"$(PROG_NAME)\" -Iinclude/
CFLAGS += $(shell pkg-config --cflags gtk+-3.0)
ifdef DEBUG
	CFLAGS += -g
endif
ifeq ($(OS), Windows_NT)
	CFLAGS += -mwindows
endif
LINK_FLAGS := $(shell pkg-config --libs gtk+-3.0)
build:
	$(CC) -g $(CFLAGS) -c $(SRC)
	mkdir -p $(OBJ_DIR) && mv ./*.o $(OBJ_DIR)
	$(CC) $(OBJ_FILES) -o $(PROG_NAME) $(LINK_FLAGS)
	chmod 755 $(PROG_NAME)
	./$(PROG_NAME)

clean:
	rm -f $(PROG_NAME)
	rm -rf $(OBJ_DIR)
