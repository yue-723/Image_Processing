target1 = hw1_flip
# target2 = hw2_resolution
# target3 = hw3_scaling
CC = g++
INCLUDE = bmp.hpp 

all: $(target1) 

$(target1): $(target1).cpp $(INCLUDE)
	$(CC) $(target1).cpp $(INCLUDE) -o $(target1)

# $(target2): $(target2).cpp $(INCLUDE)
# 	$(CC) $(target2).cpp -I $(INCLUDE) -o $(target2)
	
# $(target3): $(target3).cpp $(INCLUDE)
# 	$(CC) $(targe3).cpp -I $(INCLUDE) -o $(target3)

.PHONY: clean
clean:
	rm -r $(target1) output_bmp/*.bmp
	
run: $(target1)
	./$(target1) $(IMAGE_PATH)