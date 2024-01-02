target1 = hw4_ImageRestoration

CC = g++
INCLUDE = bmp.hpp 
CFLAGS += `pkg-config opencv --cflags --libs`
CFLAGS += -Wall
CFLAGS += -std=c++14

all: hw4_ImageRestoration

$(target1): $(target1).cpp $(INCLUDE)
	$(CC) $(target1).cpp $(INCLUDE) $(CFLAGS) -o $(target1).out
	
.PHONY: clean
clean:
	rm -rf *.out output_bmp/*.bmp

run:
	./$(target1).out $(IMAGE_1) $(IMAGE_2)