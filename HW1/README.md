## 2023 NYCU DIP HW1

### Assignment

Using C++ or C, read and write the images of **BMP format**.

To implement three types of BMP image processing:
1. Image IO and flip horizontally
2. Quantization Resolution by 6-bits, 4-bits and 2-bits
3. Scaling by Bilinear Interpolation with rate 1.5

## Files tree
```
├── bmp.hpp               // BMP io
├── hw1_flip.cpp          // flip horizontally
├── hw2_resolution.cpp    // Quantization Resolution by 6-bits, 4-bits and 2-bits
├── hw3_scaling.cpp       // Up-Scaling & Down-Scaling by Bilinear Interpolation with rate 1.5
├── makefile              // MAKEFILE
├── test_bmp              // Folder for test *.bmp
|   └── *.bmp/
└── output_bmp            // Folder for output *.bmp
    └── *.bmp/              
```

## Command instruction

• Compile all of this project to generate all ***.out**  file
```
make
```
• Run all ***.out** to get result
```
make run IMAGE_PATH=$(IMAGE_PATH)
```
• Clear the project repository
```
make clean
```
• Compile a specified ***.cpp**
```
g++ $(FILENAME).cpp bmp.hpp  -o hw1_flip.out -Wall -std=c++14
```
• Run a specified ***.out**
```
./$(FILENAME).out $(IMAGE_PATH)
```
NOTE: 
- $(FILENAME) is the filename of *.cpp
- $(IMAGE_PATH) is your path of input *.bmp image