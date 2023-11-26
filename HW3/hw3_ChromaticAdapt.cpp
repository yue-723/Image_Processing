#include "bmp.hpp"
#include <cmath>
#include <algorithm>
#include <limits>

#define OutputFolder "output_bmp/output"
#define Output_Ext "_1"
using namespace std;

void ShadeOfGray(vector<vector<RGB>> &src, vector<vector<RGB>> &dst, double p)
{
    vector<vector<vector<double>>> norm_rgb(3, vector<vector<double>>(src.size(), vector<double>(src[0].size())));

    for (size_t i = 0; i < (src.size()); i++)
        for (size_t j = 0; j < (src[0].size()); j++)
        {
            norm_rgb[0][i][j] = static_cast<double>(src[i][j].B);
            norm_rgb[1][i][j] = static_cast<double>(src[i][j].G);
            norm_rgb[2][i][j] = static_cast<double>(src[i][j].R);
        }

    double mean_rgb[3] = {0};
    double avg_mean = 0;
    double total_pix = src.size() * src[0].size();
    for (int ch = 0; ch < 3; ch++)
    {
        double max_value = 0;
        double mini_value = numeric_limits<double>::max();
        for (size_t i = 0; i < src.size(); i++)
        {
            // max_value = max(max_value, max_element(norm_rgb[ch][i].begin(), norm_rgb[ch][i].end()));
            // mini_value = min(mini_value, min_element(norm_rgb[ch][i].begin(), norm_rgb[ch][i].end()));
            for (size_t j = 0; j < src[0].size(); j++)
            {
                max_value = max(max_value, norm_rgb[ch][i][j]);
                mini_value = min(mini_value, norm_rgb[ch][i][j]);
            }
        }
        for (size_t i = 0; i < src.size(); i++)
            for (size_t j = 0; j < src[0].size(); j++)
            {
                norm_rgb[ch][i][j] = pow((norm_rgb[ch][i][j] - mini_value) / (max_value - mini_value), p);
                mean_rgb[ch] += norm_rgb[ch][i][j];
            }
        avg_mean += mean_rgb[ch];
        mean_rgb[ch] = pow((mean_rgb[ch] / total_pix), 1 / p);
    }
    avg_mean = pow((avg_mean / (3 * total_pix)), 1 / p);

    vector<double> k(3, 0);
    for (int ch = 0; ch < 3; ch++)
        k[ch] = avg_mean / mean_rgb[ch];

    for (size_t i = 0; i < src.size(); i++)
        for (size_t j = 0; j < src[0].size(); j++)
        {
            dst[i][j].B = int(min((norm_rgb[0][i][j] * k[0] * 255), 255.0));
            dst[i][j].G = int(min((norm_rgb[1][i][j] * k[1] * 255), 255.0));
            dst[i][j].R = int(min((norm_rgb[2][i][j] * k[2] * 255), 255.0));
        }
}

void PerfectReflect(vector<vector<RGB>> &src, vector<vector<RGB>> &dst, float ratio)
{
    int total_Pix = src.size() * src[0].size();
    int sumHist[766] = {0}; // max(R+G+B) = 255*3 = 765, 0~765->766
    int maxVal = 0;
    vector<vector<vector<int>>> int_RGB(3, vector<vector<int>>(src.size(), vector<int>(src[0].size())));

    for (size_t i = 0; i < src.size(); i++)
        for (size_t j = 0; j < src[0].size(); j++)
        {
            int_RGB[0][i][j] = static_cast<int>(src[i][j].B);
            int_RGB[1][i][j] = static_cast<int>(src[i][j].G);
            int_RGB[2][i][j] = static_cast<int>(src[i][j].R);
        }

    for (size_t i = 0; i < src.size(); i++)
        for (size_t j = 0; j < src[0].size(); j++)
        {
            int sum = 0;
            for (int ch = 0; ch < 3; ch++)
            {
                maxVal = maxVal > int_RGB[ch][i][j] ? maxVal : int_RGB[ch][i][j];
                sum += int_RGB[ch][i][j];
            }
            sumHist[sum]++;
        }

    int cumPixel = 0;
    int threshold = 0;
    for (int i = 765; i >= 0; i--)
    {
        cumPixel += sumHist[i];
        if (cumPixel >= ratio * total_Pix)
        {
            threshold = i;
            break;
        }
    }

    int mean_rgb[3] = {0};
    int countPixels = 0;
    for (size_t i = 0; i < src.size(); i++)
        for (size_t j = 0; j < src[0].size(); j++)
        {
            int sum = 0;

            for (int ch = 0; ch < 3; ch++)
                sum += int_RGB[ch][i][j];

            if (sum > threshold)
            {
                countPixels++;
                for (int ch = 0; ch < 3; ch++)
                    mean_rgb[ch] += int_RGB[ch][i][j];
            }
        }

    for (int ch = 0; ch < 3; ch++)
        mean_rgb[ch] /= countPixels;

    for (size_t i = 0; i < src.size(); i++)
        for (size_t j = 0; j < src[0].size(); j++)
        {
            dst[i][j].B = min((int_RGB[0][i][j] * maxVal / mean_rgb[0]), 255);
            dst[i][j].G = min((int_RGB[1][i][j] * maxVal / mean_rgb[1]), 255);
            dst[i][j].R = min((int_RGB[2][i][j] * maxVal / mean_rgb[2]), 255);
        }
}

int main(int argc, char *argv[])
{
    // processing output filename and path
    string FILEPATH = argv[argc - 2];
    string output_path = (FILEPATH).substr((FILEPATH).rfind("/") + 1);
    int found = 0;
    while (!isdigit(output_path[found]))
        found++;
    output_path.erase(0, found);
    output_path.insert(output_path.size() - 4, Output_Ext);
    output_path = OutputFolder + output_path;

    BMP bmp(FILEPATH.c_str());
    vector<vector<RGB>> copy = bmp.RGB_color;

    switch (atoi(argv[argc - 1]))
    {
    case 0:
        ShadeOfGray(copy, bmp.RGB_color, 1);
        cout << "Shade Of Gray, P=1" << endl;
        break;
    case 1:
        PerfectReflect(copy, bmp.RGB_color, 0.05);
        cout << "Perfect Reflect" << endl;
        break;
    default:
        ShadeOfGray(copy, bmp.RGB_color, 1);
        cout << "Shade Of Gray, P=1" << endl;
        break;
    }

    bmp.Save(output_path.c_str()); // Save image

    return 0;
}