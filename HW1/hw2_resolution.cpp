#include "bmp.hpp"
#include <cmath>

// using namespace std;

#define OutputFolder "output_bmp/"

void do_RGB_quantization(RGB &rgb, int ratio)
{
    ratio = pow(2, (8 - ratio));
    rgb.B = ((static_cast<int>(rgb.B) + 1) / ratio) * ratio;
    rgb.G = ((static_cast<int>(rgb.G) + 1) / ratio) * ratio;
    rgb.R = ((static_cast<int>(rgb.R) + 1) / ratio) * ratio;
}

void do_ARGB_quantization(ARGB &argb, int ratio)
{
    ratio = pow(2, (8 - ratio));
    argb.B = ((static_cast<int>(argb.B) + 1) / ratio) * ratio;
    argb.G = ((static_cast<int>(argb.G) + 1) / ratio) * ratio;
    argb.R = ((static_cast<int>(argb.R) + 1) / ratio) * ratio;
}

int main(int argc, char *argv[])
{
    string FILEPATH = argv[argc - 1];
    string filename = (FILEPATH).substr((FILEPATH).rfind("/") + 1);
    string output_path = OutputFolder + filename;

    BMP bmp(FILEPATH.c_str());
    for (int ratio = 6; ratio != 0; ratio -= 2)
    {
        if (bmp.isRGB)
        {
            for (int i = 0; i < bmp.IMG_height; i++)
                for (int j = 0; j < bmp.IMG_width; j++)
                    do_RGB_quantization(bmp.RGB_color[i][j], ratio);
        }
        else
        {
            for (int i = 0; i < bmp.IMG_height; i++)
                for (int j = 0; j < bmp.IMG_width; j++)
                    do_ARGB_quantization(bmp.ARGB_color[i][j], ratio);
        }

        output_path.insert(output_path.size() - 4, "_" + to_string(ratio));
        bmp.Save(output_path.c_str());
        output_path.erase(output_path.size() - 6, 2);
    }

    return 0;
}
