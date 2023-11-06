## 2023 NYCU DIP HW2

### Assignment

Using C++ or C, read and write the images of **BMP format**.

To implement three types of BMP image processing:
1. Low-luminosity Enhancement
2. Sharpness Enhancement 
3. Denoise 

## Files tree
```
├── bmp.hpp               // BMP io
├── hw2_LightEnhance.cpp  // Low-luminosity Enhancement
├── hw2_Sharpness.cpp     // Sharpness Enhancement 
├── hw2_Denoise.cpp       // Denoise 
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