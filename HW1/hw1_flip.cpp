#include <iostream>
#include <string>
#include <vector>
#include "bmp.hpp"
using namespace std;

#define OutputFolder "output_bmp/"

int main(int argc, char *argv[])
{
    string FILEPATH = argv[argc - 1];
    string output_path = (FILEPATH).substr((FILEPATH).rfind("/") + 1);
    output_path.insert(output_path.size() - 4, "_flip");
    output_path = OutputFolder + output_path;

    BMP bmp(FILEPATH.c_str());
    if (bmp.isRGB)
    {
        vector<vector<RGB>> new_RGB;
        vector<RGB> buffer;
        for (int i = 0; i < bmp.IMG_height; i++)
        {
            for (int j = bmp.IMG_width - 1; j >= 0; j--)
                buffer.push_back(bmp.RGB_color[i][j]);
            new_RGB.push_back(buffer);
            buffer.clear();
        }
        bmp.RGB_color = new_RGB;
    }
    else
    {
        vector<vector<ARGB>> new_ARGB;
        vector<ARGB> buffer;
        for (int i = 0; i < bmp.IMG_height; i++)
        {
            for (int j = bmp.IMG_width - 1; j >= 0; j--)
                buffer.push_back(bmp.ARGB_color[i][j]);
            new_ARGB.push_back(buffer);
            buffer.clear();
        }
        bmp.ARGB_color = new_ARGB;
    }
    bmp.Save(output_path.c_str());
    return 0;
}
