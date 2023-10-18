#include "bmp.hpp"
#include <cmath>

#define OutputFolder "output_bmp/output"

void BiInter_Linear_RGB(vector<vector<RGB>> src, vector<vector<RGB>> &dst, float ratio)
{
    for (size_t i = 0; i < dst.size(); i++)
    {
        double point_x = (i + 0.5) / ratio - 0.5;
        if (point_x < 0)
            point_x = 0;
        if (point_x >= src.size() - 1)
            point_x = src.size() - 2;

        int x1 = floor(point_x);
        int x2 = ceil(point_x);
        double u = point_x - x1;

        for (size_t j = 0; j < dst[0].size(); j++)
        {
            double point_y = (j + 0.5) / ratio - 0.5;

            if (point_y < 0)
                point_y = 0;
            if (point_y >= src[0].size() - 1)
                point_y = src[0].size() - 2;

            int y1 = floor(point_y);
            int y2 = ceil(point_y);
            double v = point_y - y1;

            RGB rgb;
            rgb.B = (1 - u) * (1 - v) * static_cast<int>(src[x1][y1].B) + (1 - u) * v * static_cast<int>(src[x1][y2].B) + u * (1 - v) * static_cast<int>(src[x2][y1].B) + u * v * static_cast<int>(src[x2][y2].B);
            rgb.G = (1 - u) * (1 - v) * static_cast<int>(src[x1][y1].G) + (1 - u) * v * static_cast<int>(src[x1][y2].G) + u * (1 - v) * static_cast<int>(src[x2][y1].G) + u * v * static_cast<int>(src[x2][y2].G);
            rgb.R = (1 - u) * (1 - v) * static_cast<int>(src[x1][y1].R) + (1 - u) * v * static_cast<int>(src[x1][y2].R) + u * (1 - v) * static_cast<int>(src[x2][y1].R) + u * v * static_cast<int>(src[x2][y2].R);
            dst[i][j] = rgb;
        }
    }
}

void BiInter_Linear_ARGB(vector<vector<ARGB>> src, vector<vector<ARGB>> &dst, float ratio)
{
    for (size_t i = 0; i < dst.size(); i++)
    {
        double point_x = (i + 0.5) / ratio - 0.5;
        if (point_x < 0)
            point_x = 0;
        if (point_x >= src.size() - 1)
            point_x = src.size() - 2;

        int x1 = floor(point_x);
        int x2 = ceil(point_x);
        double u = point_x - x1;

        for (size_t j = 0; j < dst[0].size(); j++)
        {
            double point_y = (j + 0.5) / ratio - 0.5;
            if (point_y < 0)
                point_y = 0;
            if (point_y >= src[0].size() - 1)
                point_y = src[0].size() - 2;

            int y1 = floor(point_y);
            int y2 = ceil(point_y);
            double v = point_y - y1;

            ARGB argb;
            argb.B = (1 - u) * (1 - v) * static_cast<int>(src[x1][y1].B) + (1 - u) * v * static_cast<int>(src[x1][y2].B) + u * (1 - v) * static_cast<int>(src[x2][y1].B) + u * v * static_cast<int>(src[x2][y2].B);
            argb.G = (1 - u) * (1 - v) * static_cast<int>(src[x1][y1].G) + (1 - u) * v * static_cast<int>(src[x1][y2].G) + u * (1 - v) * static_cast<int>(src[x2][y1].G) + u * v * static_cast<int>(src[x2][y2].G);
            argb.R = (1 - u) * (1 - v) * static_cast<int>(src[x1][y1].R) + (1 - u) * v * static_cast<int>(src[x1][y2].R) + u * (1 - v) * static_cast<int>(src[x2][y1].R) + u * v * static_cast<int>(src[x2][y2].R);
            argb.A = (1 - u) * (1 - v) * static_cast<int>(src[x1][y1].A) + (1 - u) * v * static_cast<int>(src[x1][y2].A) + u * (1 - v) * static_cast<int>(src[x2][y1].A) + u * v * static_cast<int>(src[x2][y2].A);
            dst[i][j] = argb;
        }
    }
}

int main(int argc, char *argv[])
{
    string FILEPATH = argv[argc - 1];
    string output_path = (FILEPATH).substr((FILEPATH).rfind("/") + 1);
    output_path.erase(0, 5);
    string output_ext[2] = {"_up", "_down"};
    output_path = OutputFolder + output_path;

    float upscale_ratio = 1.5;
    float downscale_ratio = 1 / 1.5;

    float ratio[2] = {upscale_ratio, downscale_ratio};
    for (int i = 0; i < 2; i++)
    {
        BMP bmp(FILEPATH.c_str());
        int dst_H = round(ratio[i] * bmp.IMG_height);
        int dst_W = round(ratio[i] * bmp.IMG_width);
        dst_W -= dst_W % 4;
        bmp.IMG_width = dst_W;
        bmp.IMG_height = dst_H;

        if (bmp.isRGB)
        {
            vector<vector<RGB>> new_RGB;
            new_RGB.resize(dst_H, vector<RGB>(dst_W));
            BiInter_Linear_RGB(bmp.RGB_color, new_RGB, ratio[i]);
            bmp.RGB_color = new_RGB;
        }
        else
        {
            vector<vector<ARGB>> new_ARGB;
            new_ARGB.resize(dst_H, vector<ARGB>(dst_W));
            BiInter_Linear_ARGB(bmp.ARGB_color, new_ARGB, ratio[i]);
            bmp.ARGB_color = new_ARGB;
        }
        output_path.insert(output_path.size() - 4, output_ext[i]);
        bmp.Save(output_path.c_str());
        output_path.erase(output_path.size() - 7, output_ext[i].size());
    }
    return 0;
}