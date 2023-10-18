#include "bmp.hpp"
#include <cmath>

#define OutputFolder "output_bmp/"

void do_RGB_quantization(RGB &rgb, int ratio)
{
    ratio = pow(2, (8 - ratio));
    rgb.B = ((static_cast<int>(rgb.B)) / ratio) * ratio;
    rgb.G = ((static_cast<int>(rgb.G)) / ratio) * ratio;
    rgb.R = ((static_cast<int>(rgb.R)) / ratio) * ratio;
}

void do_ARGB_quantization(ARGB &argb, int ratio)
{
    ratio = pow(2, (8 - ratio));
    argb.B = ((static_cast<int>(argb.B)) / ratio) * ratio;
    argb.G = ((static_cast<int>(argb.G)) / ratio) * ratio;
    argb.R = ((static_cast<int>(argb.R)) / ratio) * ratio;
}

int main(int argc, char *argv[])
{
    string FILEPATH = argv[argc - 1];
    string filename = (FILEPATH).substr((FILEPATH).rfind("/") + 1);
    string output_path = OutputFolder + filename;

    BMP bmp(FILEPATH.c_str());
    int ratio[3] = {6, 4, 2};
    for (int r = 0; r < 3; r++)
    {
        if (bmp.isRGB)
        {
            for (int i = 0; i < bmp.IMG_height; i++)
                for (int j = 0; j < bmp.IMG_width; j++)
                    do_RGB_quantization(bmp.RGB_color[i][j], ratio[r]);
        }
        else
        {
            for (int i = 0; i < bmp.IMG_height; i++)
                for (int j = 0; j < bmp.IMG_width; j++)
                    do_ARGB_quantization(bmp.ARGB_color[i][j], ratio[r]);
        }

        output_path.insert(output_path.size() - 4, "_" + to_string(r + 1));
        bmp.Save(output_path.c_str());
        output_path.erase(output_path.size() - 6, 2);
    }

    return 0;
}
