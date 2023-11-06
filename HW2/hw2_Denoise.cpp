#include "bmp.hpp"
#include <cmath>
#include <algorithm>

#define OutputFolder "output_bmp/3_output"
using namespace std;

void smoothing_filter(vector<vector<RGB>> &src, vector<vector<RGB>> &dst)
{
    for (size_t i = 1; i < (src.size() - 1); i++)
        for (size_t j = 1; j < (src[0].size() - 1); j++)
        {
            int kernel[3] = {0};
            for (size_t row = 0; row < 3; row++)
                for (size_t col = 0; col < 3; col++)
                {
                    kernel[0] += static_cast<int>(src[i + row - 1][j + col - 1].B);
                    kernel[1] += static_cast<int>(src[i + row - 1][j + col - 1].G);
                    kernel[2] += static_cast<int>(src[i + row - 1][j + col - 1].R);
                }

            dst[i][j].B = min(max(kernel[0] / 9, 0), 255);
            dst[i][j].G = min(max(kernel[1] / 9, 0), 255);
            dst[i][j].R = min(max(kernel[2] / 9, 0), 255);
        }
}

void median_filter(vector<vector<RGB>> &src, vector<vector<RGB>> &dst)
{

    for (size_t i = 1; i < (src.size() - 1); i++)
        for (size_t j = 1; j < (src[0].size() - 1); j++)
        {
            int kernel[3][9] = {0};
            for (size_t row = 0; row < 3; row++)
                for (size_t col = 0; col < 3; col++)
                {
                    kernel[0][row * col + col] = static_cast<int>(src[i + row - 1][j + col - 1].B);
                    kernel[1][row * col + col] = static_cast<int>(src[i + row - 1][j + col - 1].G);
                    kernel[2][row * col + col] = static_cast<int>(src[i + row - 1][j + col - 1].R);
                }

            for (size_t k = 0; k < 3; k++)
                sort(kernel[k], kernel[k] + 9);

            dst[i][j].B = min(max(kernel[0][4], 0), 255);
            dst[i][j].G = min(max(kernel[1][4], 0), 255);
            dst[i][j].R = min(max(kernel[2][4], 0), 255);
        }
}

int main(int argc, char *argv[])
{
    // processing output filename and path
    string FILEPATH = argv[argc - 1];
    string output_path = (FILEPATH).substr((FILEPATH).rfind("/") + 1);
    int found = 0;
    while (!isdigit(output_path[found]))
        found++;
    output_path.erase(0, found);
    string output_ext[2] = {"_1", "_2"};
    output_path = OutputFolder + output_path;

    for (int i = 0; i < 2; i++)
    {
        BMP bmp(FILEPATH.c_str());

        vector<vector<RGB>> copy = bmp.RGB_color;
        if (i == 0)
            for (int j = 0; j < 5; j++)
            {
                smoothing_filter(copy, bmp.RGB_color);
                copy = bmp.RGB_color;
            }
        else
            median_filter(copy, bmp.RGB_color);

        output_path.insert(output_path.size() - 4, output_ext[i]);
        bmp.Save(output_path.c_str()); // Save image
        output_path.erase(output_path.size() - 6, 2);
    }

    return 0;
}