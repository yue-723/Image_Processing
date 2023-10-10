#include <iostream>
#include <string>
#include "bmp.hpp"
using namespace std;

int main(int argc, char *argv[])
{    
    const char* FILEPATH = argv[argc - 1];
    BMP bmp(FILEPATH);
    bmp.Export("output_bmp/out.bmp");
    return 0;
}
