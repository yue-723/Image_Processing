#include <iostream>
#include <string>
#include <vector>
#include "bmp.hpp"
using namespace std;

#define OutputFolder "output_bmp/"
#define OutputExtention "_flip.bmp"

int main(int argc, char *argv[])
{
    string FILEPATH = argv[argc - 1];
    int filename_startID = (FILEPATH).rfind("/") + 1;
    string filename = (FILEPATH).substr(filename_startID);
    filename = filename.substr(0, filename.size() - 4);
    string output_path = OutputFolder + filename + OutputExtention;

    BMP bmp(FILEPATH.c_str());
    if (bmp.isRGB)
    {
        vector<vector<RGB>> new_RGB;
        vector<RGB> temp;
        for (int i = 0; i < bmp.IMG_height; i++)
        {
            for (int j = bmp.IMG_width - 1; j >= 0; j--)
                temp.push_back(bmp.RGB_color[i][j]);
            new_RGB.push_back(temp);
            temp.clear();
        }
        bmp.RGB_color = new_RGB;
    }
    else
    {
        vector<vector<ARGB>> new_ARGB;
        vector<ARGB> temp;
        for (int i = 0; i < bmp.IMG_height; i++)
        {
            for (int j = bmp.IMG_width - 1; j >= 0; j--)
                temp.push_back(bmp.ARGB_color[i][j]);
            new_ARGB.push_back(temp);
            temp.clear();
        }
        bmp.ARGB_color = new_ARGB;
    }
    bmp.Save(output_path.c_str());
    return 0;
}
