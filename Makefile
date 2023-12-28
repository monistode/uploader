CC ?= g++
CFLAGS += -std=c++14 -Ofast
INCLUDE = ./include/
SRC_DIR = ./src/
BUILD = $(abspath ./build)

.PHONY: all


all: prep_dirs makeobj link


prep_dirs:
	mkdir -p build
	mkdir -p build/obj
	mkdir -p build/bin

makeobj: prep_dirs
	$(MAKE) -C $(SRC_DIR) $(MAKECMDGOALS)

link: makeobj
	$(CC) -o $(BUILD)/bin/upload $(BUILD)/obj/*.o
