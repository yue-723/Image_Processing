#include "bmp.hpp"
#include <algorithm>

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
        for (int i = 0; i < bmp.IMG_height; i++)
            reverse(bmp.RGB_color[i].begin(), bmp.RGB_color[i].end());
    }
    else
    {
        for (int i = 0; i < bmp.IMG_height; i++)
            reverse(bmp.ARGB_color[i].begin(), bmp.ARGB_color[i].end());
    }
    bmp.Save(output_path.c_str());

    return 0;
}
