IMAGE_DIR_1 = test_bmp/*.bmp
IMAGE_DIR_2 = output_bmp/*.bmp

target1 = hw3_ChromaticAdapt
target2 = hw3_ImageEnhance
AWB_TYPE = 0
Enhance_TYPE = 0

CC = g++
INCLUDE = bmp.hpp 
CFLAGS += -Wall
CFLAGS += -std=c++14


all: $(target1) $(target2)

$(target1): $(target1).cpp $(INCLUDE)
	$(CC) $(target1).cpp $(INCLUDE) $(CFLAGS) -o $(target1).out

$(target2): $(target2).cpp $(INCLUDE)
	$(CC) $(target2).cpp $(INCLUDE) $(CFLAGS) -o $(target2).out 


.PHONY: clean
clean:
	rm -rf *.out output_bmp/*.bmp

task1:
ifeq ("$(wildcard *.out)","")
	make
endif 
	(for F in $(wildcard $(IMAGE_DIR_1)); do ./$(target1).out $$F $(AWB_TYPE); done)

task2:
	(for F in $(wildcard $(IMAGE_DIR_2)); do ./$(target2).out $$F $(Enhance_TYPE); done)

run: task1 task2
	echo "Done!" 


