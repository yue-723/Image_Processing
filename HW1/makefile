target1 = hw1_flip
# target2 = hw2_resolution
# target3 = hw3_scaling
CC = g++
INCLUDE = bmp.hpp 
CFLAGS+= -std=c++14
CFLAGS+= -Wall

all: hw1_flip

$(target1): hw1_flip.cpp $(INCLUDE)
	$(CC) hw1_flip.cpp $(INCLUDE) -o hw1_flip $(CFLAGS)

# $(target2): $(target2).cpp $(INCLUDE)
# 	$(CC) $(target2).cpp -I $(INCLUDE) -o $(target2)
	
# $(target3): $(target3).cpp $(INCLUDE)
# 	$(CC) $(targe3).cpp -I $(INCLUDE) -o $(target3)

.PHONY: clean
clean:
	rm -r hw1_flip output_bmp/*.bmp
	
run: hw1_flip
	./hw1_flip $(IMAGE_PATH)