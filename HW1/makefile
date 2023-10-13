# target1 = hw1_flip
# target2 = hw2_resolution
# target3 = hw3_scaling
CC = g++
INCLUDE = bmp.hpp 
CFLAGS += -std=c++14
CFLAGS += -Wall

all: hw1_flip hw2_resolution

hw1_flip: hw1_flip.cpp $(INCLUDE)
	$(CC) hw1_flip.cpp $(INCLUDE) -o hw1_flip.out $(CFLAGS)

hw2_resolution: hw2_resolution.cpp $(INCLUDE)
	$(CC) hw2_resolution.cpp $(INCLUDE) -o hw2_resolution.out $(CFLAGS)
	
.PHONY: clean
clean:
	rm -rf *.out output_bmp/*.bmp

run: hw1_flip hw2_resolution
	./hw1_flip.out $(IMAGE_PATH)
	./hw2_resolution.out $(IMAGE_PATH)