CC ?= g++
CFLAGS += -std=c++14 -Ofast
INCLUDE = ../include/
BUILD = ../build
BUILD_FILES = $(wildcard ./*.cpp)

.PHONY: all

all: build_files

$(BUILD_FILES):
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $*.cpp -o "$(BUILD)/obj/$*.o"
	echo $(CC) $(CFLAGS) -I$(INCLUDE) -c $*.cpp -o "$(BUILD)/obj/$*.o"

build_files: $(BUILD_FILES)
	echo "Object generation done"
