#include "bmp.hpp"
#include <cmath>
// #include <algorithm>

#define OutputFolder "output_bmp/2_output"

int Laplacian_kernel[3][3] = {0, -1, 0,
                              -1, 4, -1,
                              0, -1, 0};

int Sharpening[3][3] = {-1, -1, -1,
                        -1, 9, -1,
                        -1, -1, -1};

int SOBEL_X[3][3] = {-1, 0, 1,
                     -2, 0, 2,
                     -1, 0, 1};

int SOBEL_Y[3][3] = {-1, -2, -1,
                     0, 0, 0,
                     1, 2, 1};

void Sharpening_base(vector<vector<RGB>> src, vector<vector<RGB>> &dst)
{
    for (size_t i = 1; i < (src.size() - 1); i++)
        for (size_t j = 1; j < (src[0].size() - 1); j++)
        {
            int kernel[3] = {0};
            for (size_t row = 0; row < 3; row++)
                for (size_t col = 0; col < 3; col++)
                {
                    kernel[0] += static_cast<int>(src[i + row - 1][j + col - 1].B) * Sharpening[row][col];
                    kernel[1] += static_cast<int>(src[i + row - 1][j + col - 1].G) * Sharpening[row][col];
                    kernel[2] += static_cast<int>(src[i + row - 1][j + col - 1].R) * Sharpening[row][col];
                }

            dst[i][j].B = min(max(kernel[0], 0), 255);
            dst[i][j].G = min(max(kernel[1], 0), 255);
            dst[i][j].R = min(max(kernel[2], 0), 255);
        }
}

typedef struct
{
    int B;
    int G;
    int R;
} RGB_int;

void Laplacian_filter(vector<vector<RGB>> src, vector<vector<RGB_int>> &dst)
{
    for (size_t i = 1; i < (src.size() - 1); i++)
        for (size_t j = 1; j < (src[0].size() - 1); j++)
        {
            int kernel[3] = {0};
            for (size_t row = 0; row < 3; row++)
                for (size_t col = 0; col < 3; col++)
                {
                    kernel[0] += static_cast<int>(src[i + row - 1][j + col - 1].B) * Laplacian_kernel[row][col];
                    kernel[1] += static_cast<int>(src[i + row - 1][j + col - 1].G) * Laplacian_kernel[row][col];
                    kernel[2] += static_cast<int>(src[i + row - 1][j + col - 1].R) * Laplacian_kernel[row][col];
                }

            dst[i][j].B = min(max(kernel[0], 0), 255);
            dst[i][j].G = min(max(kernel[1], 0), 255);
            dst[i][j].R = min(max(kernel[2], 0), 255);
        }
}
void Sobel_filter(vector<vector<RGB>> src, vector<vector<RGB_int>> &dst)
{
    for (size_t i = 1; i < (src.size() - 1); i++)
        for (size_t j = 1; j < (src[0].size() - 1); j++)
        {
            int G_X[3] = {0};
            int G_Y[3] = {0};
            for (size_t row = 0; row < 3; row++)
                for (size_t col = 0; col < 3; col++)
                {
                    G_X[0] += static_cast<int>(src[i + row - 1][j + col - 1].B) * SOBEL_X[row][col];
                    G_X[1] += static_cast<int>(src[i + row - 1][j + col - 1].G) * SOBEL_X[row][col];
                    G_X[2] += static_cast<int>(src[i + row - 1][j + col - 1].R) * SOBEL_X[row][col];

                    G_Y[0] += static_cast<int>(src[i + row - 1][j + col - 1].B) * SOBEL_Y[row][col];
                    G_Y[1] += static_cast<int>(src[i + row - 1][j + col - 1].G) * SOBEL_Y[row][col];
                    G_Y[2] += static_cast<int>(src[i + row - 1][j + col - 1].R) * SOBEL_Y[row][col];
                }

            dst[i][j].B = min(max(int(sqrt(pow(G_X[0], 2) + pow(G_Y[0], 2))), 0), 255);
            dst[i][j].G = min(max(int(sqrt(pow(G_X[1], 2) + pow(G_Y[1], 2))), 0), 255);
            dst[i][j].R = min(max(int(sqrt(pow(G_X[2], 2) + pow(G_Y[2], 2))), 0), 255);
        }
}

void UnsharpMask(vector<vector<RGB>> src, vector<vector<RGB>> &dst)
{
    size_t H = src.size();
    size_t W = src[0].size();

    vector<vector<RGB_int>> Lap(H, vector<RGB_int>(W));
    Laplacian_filter(src, Lap);
    // for (size_t i = 1; i < (H - 1); i++)
    //     for (size_t j = 1; j < (W - 1); j++)
    //     {
    //         dst[i][j].B = min(max(static_cast<int>(dst[i][j].B) + Lap[i][j].B, 0), 255);
    //         dst[i][j].G = min(max(static_cast<int>(dst[i][j].G) + Lap[i][j].G, 0), 255);
    //         dst[i][j].R = min(max(static_cast<int>(dst[i][j].R) + Lap[i][j].R, 0), 255);
    //     }

    vector<vector<RGB_int>> sobel(H, vector<RGB_int>(W));
    Sobel_filter(src, sobel);
    // for (size_t i = 1; i < (H - 1); i++)
    //     for (size_t j = 1; j < (W - 1); j++)
    //     {
    //         dst[i][j].B = min(max(static_cast<int>(dst[i][j].B) + sobel[i][j].B, 0), 255);
    //         dst[i][j].G = min(max(static_cast<int>(dst[i][j].G) + sobel[i][j].G, 0), 255);
    //         dst[i][j].R = min(max(static_cast<int>(dst[i][j].R) + sobel[i][j].R, 0), 255);
    //     }

    vector<vector<RGB_int>> mean(H, vector<RGB_int>(W));
    for (size_t i = 1; i < (H - 1); i++)
        for (size_t j = 1; j < (W - 1); j++)
        {
            int m[3] = {0};
            for (size_t row = 0; row < 3; row++)
                for (size_t col = 0; col < 3; col++)
                {
                    m[0] += static_cast<int>(sobel[i + row - 1][j + col - 1].B);
                    m[1] += static_cast<int>(sobel[i + row - 1][j + col - 1].G);
                    m[2] += static_cast<int>(sobel[i + row - 1][j + col - 1].R);
                }

            mean[i][j].B = min(max(m[0] / 9, 0), 255);
            mean[i][j].G = min(max(m[1] / 9, 0), 255);
            mean[i][j].R = min(max(m[2] / 9, 0), 255);
        }
    // for (size_t i = 1; i < (H - 1); i++)
    //     for (size_t j = 1; j < (W - 1); j++)
    //     {
    //         dst[i][j].B = min(max(static_cast<int>(dst[i][j].B) + (mean[i][j].B), 0), 255);
    //         dst[i][j].G = min(max(static_cast<int>(dst[i][j].G) + (mean[i][j].G), 0), 255);
    //         dst[i][j].R = min(max(static_cast<int>(dst[i][j].R) + (mean[i][j].R), 0), 255);
    //     }
    for (size_t i = 1; i < (H - 1); i++)
        for (size_t j = 1; j < (W - 1); j++)
        {
            dst[i][j].B = min(max(static_cast<int>(dst[i][j].B) + int(Lap[i][j].B * (mean[i][j].B / 255.0)), 0), 255);
            dst[i][j].G = min(max(static_cast<int>(dst[i][j].G) + int(Lap[i][j].G * (mean[i][j].G / 255.0)), 0), 255);
            dst[i][j].R = min(max(static_cast<int>(dst[i][j].R) + int(Lap[i][j].R * (mean[i][j].R / 255.0)), 0), 255);
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
            UnsharpMask(copy, bmp.RGB_color);
        else
            Sharpening_base(copy, bmp.RGB_color);

        output_path.insert(output_path.size() - 4, output_ext[i]);
        bmp.Save(output_path.c_str()); // Save image
        output_path.erase(output_path.size() - 6, 2);
    }
    return 0;
}
