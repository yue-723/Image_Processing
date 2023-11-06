# target1 = hw2_LightEnhance
# target2 = hw2_Sharpness
# target3 = hw2_Denoise
CC = g++
INCLUDE = bmp.hpp 
CFLAGS += -Wall
CFLAGS += -std=c++14


all: hw2_LightEnhance hw2_Sharpness hw2_Denoise

hw2_LightEnhance: hw2_LightEnhance.cpp $(INCLUDE)
	$(CC) hw2_LightEnhance.cpp $(INCLUDE) $(CFLAGS) -o hw2_LightEnhance.out

hw2_Sharpness: hw2_Sharpness.cpp $(INCLUDE)
	$(CC) hw2_Sharpness.cpp $(INCLUDE) $(CFLAGS) -o hw2_Sharpness.out 

hw2_Denoise: hw2_Denoise.cpp $(INCLUDE)
	$(CC) hw2_Denoise.cpp $(INCLUDE) $(CFLAGS) -o hw2_Denoise.out 
	
.PHONY: clean
clean:
	rm -rf *.out output_bmp/*.bmp

run:
	./hw2_LightEnhance.out $(IMAGE_PATH)
	./hw2_Sharpness.out $(IMAGE_PATH)
	./hw2_Denoise.out $(IMAGE_PATH)
