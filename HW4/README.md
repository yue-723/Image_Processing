## 2023 NYCU DIP HW4

### Assignment

Using C++ or C, read and write the images of **BMP format**.

To implement BMP image processing:

1. Image restoration by Wiener filter

## Files tree

```
├── bmp.hpp                   // BMP io
├── hw4_ImageRestoration.cpp  // Image Restoration
├── makefile                  // MAKEFILE
├── test_bmp                  // Folder for test *.bmp
|   └── *.bmp/
└── output_bmp                // Folder for output *.bmp
    └── *.bmp/
```

## Command instruction

• Compile all of this project to generate all **\*.out** file

```
make
```

• Get all the result quickly

```
make run IMAGE_1=$(IMAGE_PATH) IMAGE_2=$(IMAGE_PATH)
```

• Clear the project repository

```
make clean
```

• Compile a specified **\*.cpp**

```
g++ $(CPP_FILE).cpp bmp.hpp  -o $(CPP_FILE).out `pkg-config opencv --cflags --libs` -Wall -std=c++14
```

• Run **hw4_ImageRestoration.out**

```
./hw4_ImageRestoration.out
```

NOTE:

- **_$(CPP_FILE)_** is the filename of **\*.cpp**
- **_$(IMAGE_PATH)_** is your path of **\*.bmp image**
