#include "bmp.hpp"
#include <cmath>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;

#define OutputFolder "output_bmp/output"
#define LEN 30
#define THETA 138
#define SNR 25

int Sharpening[3][3] = {-1, -1, -1,
                        -1, 9, -1,
                        -1, -1, -1};

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

void bmp2mat(BMP &bmp, vector<cv::Mat> &mat)
{
    for (int i = 0; i < bmp.IMG_height; i++)
        for (int j = 0; j < bmp.IMG_width; j++)
        {
            mat[0].at<float>(i, j) = static_cast<float>(bmp.RGB_color[i][j].B);
            mat[1].at<float>(i, j) = static_cast<float>(bmp.RGB_color[i][j].G);
            mat[2].at<float>(i, j) = static_cast<float>(bmp.RGB_color[i][j].R);
        }
}

void mat2bmp(vector<cv::Mat> &mat, BMP &bmp)
{
    for (int i = 0; i < bmp.IMG_height; i++)
        for (int j = 0; j < bmp.IMG_width; j++)
        {
            bmp.RGB_color[i][j].B = static_cast<uint8_t>(mat[0].at<float>(i, j));
            bmp.RGB_color[i][j].G = static_cast<uint8_t>(mat[1].at<float>(i, j));
            bmp.RGB_color[i][j].R = static_cast<uint8_t>(mat[2].at<float>(i, j));
        }
}

void PSNR(vector<vector<RGB>> blur, vector<vector<RGB>> clr)
{
    int total_Pix = blur.size() * blur[0].size();
    double mse = 0;

    for (size_t i = 0; i < blur.size(); i++)
        for (size_t j = 0; j < blur[0].size(); j++)
        {
            mse += pow((static_cast<double>(blur[i][j].R) - static_cast<double>(clr[i][j].R)), 2);
            mse += pow((static_cast<double>(blur[i][j].G) - static_cast<double>(clr[i][j].G)), 2);
            mse += pow((static_cast<double>(blur[i][j].B) - static_cast<double>(clr[i][j].B)), 2);
        }
    mse /= (3 * total_Pix);

    if (mse < 1e-10)
        cout << "PSNR: 0" << endl;
    else
        cout << "PSNR: " << 10.0 * log10(pow(255, 2) / mse) << endl;
}

void Estimate_PSF(cv::Mat &outputImg, cv::Size filterSize, int len, double theta)
{
    cv::Mat h(filterSize, CV_32FC1, cv::Scalar(0));
    cv::Point point(filterSize.width / 2, filterSize.height / 2);
    cv::ellipse(h, point, cv::Size(0, cvRound(float(len) / 2.0)), 90.0 - theta, 0, 360, cv::Scalar(255), cv::FILLED);
    cv::Scalar summa = cv::sum(h);
    outputImg = h / summa[0];
}

void DFT_Shift(const cv::Mat &inputImg, cv::Mat &outputImg)
{
    outputImg = inputImg.clone();
    int cx = outputImg.cols / 2;
    int cy = outputImg.rows / 2;
    cv::Mat q0(outputImg, cv::Rect(0, 0, cx, cy));
    cv::Mat q1(outputImg, cv::Rect(cx, 0, cx, cy));
    cv::Mat q2(outputImg, cv::Rect(0, cy, cx, cy));
    cv::Mat q3(outputImg, cv::Rect(cx, cy, cx, cy));
    cv::Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

void MotionDeblur(const cv::Mat &inputImg, cv::Mat &outputImg, const cv::Mat &H)
{
    cv::Mat planes[2] = {cv::Mat_<float>(inputImg.clone()), cv::Mat::zeros(inputImg.size(), CV_32FC1)};
    cv::Mat complexI;
    cv::merge(planes, 2, complexI);
    cv::dft(complexI, complexI, cv::DFT_SCALE);
    cv::Mat planesH[2] = {cv::Mat_<float>(H.clone()), cv::Mat::zeros(H.size(), CV_32FC1)};
    cv::Mat complexH;
    cv::merge(planesH, 2, complexH);
    cv::Mat complexIH;
    cv::mulSpectrums(complexI, complexH, complexIH, 0);
    cv::idft(complexIH, complexIH);
    cv::split(complexIH, planes);
    outputImg = planes[0];
}

void Gen_WnrFilter(const cv::Mat &input_h_PSF, cv::Mat &output_G, double nsr)
{
    cv::Mat h_PSF_shifted;
    DFT_Shift(input_h_PSF, h_PSF_shifted);
    cv::Mat planes[2] = {cv::Mat_<float>(h_PSF_shifted.clone()), cv::Mat::zeros(h_PSF_shifted.size(), CV_32FC1)};
    cv::Mat complexI;
    cv::merge(planes, 2, complexI);
    cv::dft(complexI, complexI);
    cv::split(complexI, planes);
    cv::Mat denom;
    cv::pow(cv::abs(planes[0]), 2, denom);
    denom += nsr;
    cv::divide(planes[0], denom, output_G);
}

void normalize(cv::Mat &output)
{
    for (int i = 0; i < output.rows; i++)
        for (int j = 0; j < output.cols; j++)
            output.at<float>(i, j) = min(max(output.at<float>(i, j), float(0)), float(255));
}

int main(int argc, char *argv[])
{
    // processing output filename and path
    string IMAGE_PATH[] = {argv[argc - 2], argv[argc - 1]};
    string CLEAR_IMG = "test_bmp/input1_ori.bmp";
    BMP clear_img(CLEAR_IMG.c_str());
    for (int i = 0; i < 2; i++)
    {
        string output_path = (IMAGE_PATH[i]).substr((IMAGE_PATH[i]).rfind("/") + 1);
        int found = 0;
        while (!isdigit(output_path[found]))
            found++;
        output_path.erase(0, found);
        output_path = OutputFolder + output_path;

        BMP blur_img(IMAGE_PATH[i].c_str());

        vector<cv::Mat> blur_mat;

        for (int i = 0; i < 3; i++)
            blur_mat.push_back(cv::Mat::zeros(blur_img.IMG_height, blur_img.IMG_width, CV_32FC1));

        bmp2mat(blur_img, blur_mat);

        vector<cv::Mat> res_img;
        for (int i = 0; i < 3; i++)
        {
            cv::Mat blur_DFT, PSF, WnrFilter;
            Estimate_PSF(PSF, blur_mat[i].size(), LEN, THETA);
            Gen_WnrFilter(PSF, WnrFilter, double(1.0 / SNR));
            cv::Mat output;
            MotionDeblur(blur_mat[i], output, WnrFilter);
            normalize(output);
            res_img.push_back(output);
        }

        mat2bmp(res_img, blur_img);

        vector<vector<RGB>> copy = blur_img.RGB_color;
        Sharpening_base(copy, blur_img.RGB_color);

        if (i == 0)
            PSNR(blur_img.RGB_color, clear_img.RGB_color);

        blur_img.Save(output_path.c_str()); // Save image
    }

    return 0;
}