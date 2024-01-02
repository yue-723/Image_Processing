# target1 = hw1_flip
# target2 = hw1_resolution
# target3 = hw1_scaling
CC = g++
INCLUDE = bmp.hpp 
CFLAGS += -Wall
CFLAGS += -std=c++14


all: hw1_flip hw1_resolution hw1_scaling

hw1_flip: hw1_flip.cpp $(INCLUDE)
	$(CC) hw1_flip.cpp $(INCLUDE) -o hw1_flip.out $(CFLAGS)

hw1_resolution: hw1_resolution.cpp $(INCLUDE)
	$(CC) hw1_resolution.cpp $(INCLUDE) -o hw1_resolution.out $(CFLAGS)

hw1_scaling: hw1_scaling.cpp $(INCLUDE)
	$(CC) hw1_scaling.cpp $(INCLUDE) -o hw1_scaling.out $(CFLAGS)
	
.PHONY: clean
clean:
	rm -rf *.out output_bmp/*.bmp

run:
	./hw1_flip.out $(IMAGE_PATH)
	./hw1_resolution.out $(IMAGE_PATH)
	./hw1_scaling.out $(IMAGE_PATH)