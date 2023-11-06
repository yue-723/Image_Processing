#include "bmp.hpp"
#include <algorithm>

#define OutputFolder "output_bmp/1_output"

typedef struct
{
    double H;
    double S;
    double L;
} HSL;

HSL RGB2HSL(int r, int g, int b)
{
    HSL hsl;
    // Normalize RGB values
    double normR = r / 255.0;
    double normG = g / 255.0;
    double normB = b / 255.0;

    // Find the maximum and minimum values
    double maxVal = max({normR, normG, normB});
    double minVal = min({normR, normG, normB});

    // Calculate lightness
    hsl.L = (maxVal + minVal) / 2.0;

    // Check if the color is grayscale
    if (maxVal == minVal)
    {
        hsl.H = 0.0;
        hsl.S = 0.0;
    }
    else
    {
        // Calculate saturation
        if (hsl.L < 0.5)
        {
            hsl.S = (maxVal - minVal) / (maxVal + minVal);
        }
        else
        {
            hsl.S = (maxVal - minVal) / (2.0 - maxVal - minVal);
        }

        // Calculate hue
        double delta = maxVal - minVal;
        if (maxVal == normR)
        {
            hsl.H = (normG - normB) / delta;
        }
        else if (maxVal == normG)
        {
            hsl.H = 2.0 + (normB - normR) / delta;
        }
        else
        {
            hsl.H = 4.0 + (normR - normG) / delta;
        }

        hsl.H *= 60.0;
        if (hsl.H < 0.0)
        {
            hsl.H += 360.0;
        }
    }
    return hsl;
}

RGB HSL2RGB(HSL hsl)
{
    float R, G, B;
    RGB rgb;

    if (hsl.S == 0)
    {
        // achromatic color (gray scale)
        R = G = B = hsl.L * 255.f;
    }
    else
    {
        float q = (hsl.L < 0.5f) ? (hsl.L * (1.0f + hsl.S)) : (hsl.L + hsl.S - (hsl.L * hsl.S));
        float p = (2.0f * hsl.L) - q;
        float Hk = hsl.H / 360.0f;
        float T[3];
        T[0] = Hk + 0.3333333f; // Tr	0.3333333f=1.0/3.0
        T[1] = Hk;              // Tb
        T[2] = Hk - 0.3333333f; // Tg

        for (int i = 0; i < 3; i++)
        {
            if (T[i] < 0)
                T[i] += 1.0f;
            if (T[i] > 1)
                T[i] -= 1.0f;
            if ((T[i] * 6) < 1)
            {
                T[i] = p + ((q - p) * 6.0f * T[i]);
            }
            else if ((T[i] * 2.0f) < 1) //(1.0/6.0)<=T[i] && T[i]<0.5
            {
                T[i] = q;
            }
            else if ((T[i] * 3.0f) < 2) // 0.5<=T[i] && T[i]<(2.0/3.0)
            {
                T[i] = p + (q - p) * ((2.0f / 3.0f) - T[i]) * 6.0f;
            }
            else
                T[i] = p;
        }

        R = T[0] * 255.0f;
        G = T[1] * 255.0f;
        B = T[2] * 255.0f;
    }

    rgb.R = (uint8_t)((R > 255) ? 255 : ((R < 0) ? 0 : R));
    rgb.G = (uint8_t)((G > 255) ? 255 : ((G < 0) ? 0 : G));
    rgb.B = (uint8_t)((B > 255) ? 255 : ((B < 0) ? 0 : B));

    return rgb;
}

void Hist_equl(vector<vector<RGB>> src, vector<vector<RGB>> &dst)
{
    int Pix[3][256] = {0};
    double RGB_pdf[3][256] = {0};
    double RGB_cdf[3][256] = {0};
    int result[3][256] = {0};
    int total_pix = src.size() * src[0].size();

    for (size_t i = 0; i < src.size(); i++)
        for (size_t j = 0; j < src[0].size(); j++)
        {
            RGB rgb = src[i][j];
            Pix[0][static_cast<int>(rgb.B)]++;
            Pix[1][static_cast<int>(rgb.G)]++;
            Pix[2][static_cast<int>(rgb.R)]++;
        }

    for (int i = 0; i < 256; i++)
    {
        RGB_pdf[0][i] = ((double)Pix[0][i] / total_pix);
        RGB_pdf[1][i] = ((double)Pix[1][i] / total_pix);
        RGB_pdf[2][i] = ((double)Pix[2][i] / total_pix);
    }

    RGB_cdf[0][0] = RGB_pdf[0][0];
    RGB_cdf[1][0] = RGB_pdf[1][0];
    RGB_cdf[2][0] = RGB_pdf[2][0];

    for (int i = 1; i < 256; i++)
    {
        RGB_cdf[0][i] = RGB_cdf[0][i - 1] + RGB_pdf[0][i];
        RGB_cdf[1][i] = RGB_cdf[1][i - 1] + RGB_pdf[1][i];
        RGB_cdf[2][i] = RGB_cdf[2][i - 1] + RGB_pdf[2][i];
    }

    for (int i = 0; i < 256; i++)
    {
        result[0][i] = (int)(255 * RGB_cdf[0][i] + 0.5);
        result[1][i] = (int)(255 * RGB_cdf[1][i] + 0.5);
        result[2][i] = (int)(255 * RGB_cdf[2][i] + 0.5);
    }
    for (size_t i = 0; i < dst.size(); i++)
        for (size_t j = 0; j < dst[0].size(); j++)
        {
            RGB rgb = src[i][j];

            dst[i][j].B = result[0][static_cast<int>(rgb.B)];
            dst[i][j].G = result[1][static_cast<int>(rgb.G)];
            dst[i][j].R = result[2][static_cast<int>(rgb.R)];
        }
}

void Hist_equl_hsl(vector<vector<RGB>> src, vector<vector<RGB>> &dst)
{
    int Pix[256] = {0};
    double l_pdf[256] = {0};
    double l_cdf[256] = {0};
    int result[256] = {0};
    int total_pix = src.size() * src[0].size();

    for (size_t i = 0; i < src.size(); i++)
        for (size_t j = 0; j < src[0].size(); j++)
        {
            RGB rgb = src[i][j];
            HSL hsl = RGB2HSL(static_cast<float>(rgb.R), static_cast<float>(rgb.G), static_cast<float>(rgb.B));
            Pix[int(hsl.L * 255)]++;
        }

    for (int i = 0; i < 256; i++)
        l_pdf[i] = ((double)Pix[i] / total_pix);

    l_cdf[0] = l_pdf[0];

    for (int i = 1; i < 256; i++)
        l_cdf[i] = l_cdf[i - 1] + l_pdf[i];

    for (int i = 0; i < 256; i++)
        result[i] = (255 * l_cdf[i] + 0.5);

    for (size_t i = 0; i < dst.size(); i++)
        for (size_t j = 0; j < dst[0].size(); j++)
        {
            RGB rgb = src[i][j];
            HSL hsl = RGB2HSL(static_cast<int>(rgb.R), static_cast<int>(rgb.G), static_cast<int>(rgb.B));

            hsl.L = result[int(hsl.L * 255)] / 255.0;

            rgb = HSL2RGB(hsl);

            dst[i][j] = rgb;
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
            Hist_equl(copy, bmp.RGB_color);
        else
            Hist_equl_hsl(copy, bmp.RGB_color);

        output_path.insert(output_path.size() - 4, output_ext[i]);
        bmp.Save(output_path.c_str()); // Save image
        output_path.erase(output_path.size() - 6, 2);
    }

    return 0;
}
