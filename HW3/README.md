## 2023 NYCU DIP HW3

### Assignment

Using C++ or C, read and write the images of **BMP format**.

To implement two types of BMP image processing:

1. Chromatic Adaptation
2. Image Enhancement

## Files tree

```
├── bmp.hpp                 // BMP io
├── hw3_ChromaticAdapt.cpp  // Chromatic Adaptation
├── hw3_ImageEnhance.cpp    // Image Enhancement
├── makefile                // MAKEFILE
├── test_bmp                // Folder for test *.bmp
|   └── *.bmp/
└── output_bmp              // Folder for output *.bmp
    └── *.bmp/
```

## Command instruction

• Compile all of this project to generate all **\*.out** file

```
make
```

• Get all the result quickly

```
make run AWB_TYPE=$(AWB_TYPE) Enhance_TYPE=$(Enhance_TYPE)
```

• Clear the project repository

```
make clean
```

• Compile a specified **\*.cpp**

```
g++ $(CPP_FILE).cpp bmp.hpp  -o $(CPP_FILE).out -Wall -std=c++14
```

• Run **hw3_ChromaticAdapt.out**

```
./hw3_ChromaticAdapt.out $(IMAGE_PATH) $(AWB_TYPE)
```

• Run **hw3_ImageEnhance.out**

```
./hw3_ImageEnhance.out $(IMAGE_PATH) $(Enhance_TYPE)
```

NOTE:

- **_$(AWB_TYPE)_** is the type of **Auto White Balance**:
  - AWB_TYPE=0: Shade Of Gray, p is set as 1 (Equal Gray world method)
  - AWB_TYPE=1: Perfect Reflect
- **_$(Enhance_TYPE)_** is the type of **Image Enhancement**:
  - Enhance_TYPE=0: Sharpness by Unsharp Mask
  - Enhance_TYPE=1: Contrast Enhancement by Histogram Equalization
  - Enhance_TYPE=2: Denoise by Median Filter
- **_$(CPP_FILE)_** is the filename of **\*.cpp**
- **_$(IMAGE_PATH)_** is your path of **\*.bmp image**
