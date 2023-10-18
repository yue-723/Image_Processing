#include "bmp.hpp"
#include <cmath>

#define OutputFolder "output_bmp/output"

void do_RGB_quantization(RGB &rgb, int ratio)
{
    ratio = pow(2, (8 - ratio)); // get ratio of resolution
    rgb.B = ((static_cast<int>(rgb.B)) / ratio) * ratio;
    rgb.G = ((static_cast<int>(rgb.G)) / ratio) * ratio;
    rgb.R = ((static_cast<int>(rgb.R)) / ratio) * ratio;
}

void do_ARGB_quantization(ARGB &argb, int ratio)
{
    ratio = pow(2, (8 - ratio)); // get ratio of resolution
    argb.B = ((static_cast<int>(argb.B)) / ratio) * ratio;
    argb.G = ((static_cast<int>(argb.G)) / ratio) * ratio;
    argb.R = ((static_cast<int>(argb.R)) / ratio) * ratio;
    argb.A = ((static_cast<int>(argb.A)) / ratio) * ratio;
}

int main(int argc, char *argv[])
{
    // processing output path
    string FILEPATH = argv[argc - 1];
    string output_path = (FILEPATH).substr((FILEPATH).rfind("/") + 1);
    output_path.erase(0, 5);
    output_path = OutputFolder + output_path;

    BMP bmp(FILEPATH.c_str()); // loading *.bmp

    int ratio[3] = {6, 4, 2}; // Quantization Resolution by 6-bits, 4-bits and 2-bits

    // Quantization Resolution
    for (int r = 0; r < 3; r++)
    {
        if (bmp.isRGB) // check pixel depth
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
        // processing output filename and save
        output_path.insert(output_path.size() - 4, "_" + to_string(r + 1));
        bmp.Save(output_path.c_str());
        output_path.erase(output_path.size() - 6, 2);
    }

    return 0;
}
