#include <iostream>
#include <opencv2\imgcodecs.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <vector>

using namespace cv;

// Auto White Balance - Gray World Algorithm
int AWB_GrayWorld(InputArray src, OutputArray dst)
{
    CV_Assert(src.channels() == 3, "AWB_GrayWorld() input image must be 3 channels!");

    Mat mSrc = src.getMat();
    if (mSrc.empty())
    {
        std::cout << "AWB_GrayWorld() input image is empty!" << std::endl;
        return -1;
    }

    dst.create(mSrc.size(), mSrc.type());
    Mat mDst = dst.getMat();

    if (mDst.empty())
    {
        std::cout << "AWB_GrayWorld() create dst image failed!" << std::endl;
        return -1;
    }

    // 对输入src图像进行RGB分离
    std::vector<Mat> splitedBGR;
    splitedBGR.reserve(3);

    split(mSrc, splitedBGR);

    // 分别计算R/G/B图像像素值均值
    double meanR = 0, meanG = 0, meanB = 0;
    meanB = mean(splitedBGR[0])[0];
    meanG = mean(splitedBGR[1])[0];
    meanR = mean(splitedBGR[2])[0];

    // 计算R/G/B图像的增益
    double gainR = 0, gainG = 0, gainB = 0;
    gainR = (meanR + meanG + meanB) / (3 * meanR);
    gainG = (meanR + meanG + meanB) / (3 * meanG);
    gainB = (meanR + meanG + meanB) / (3 * meanB);

    // 计算增益后R/G/B图像
    splitedBGR[0] = splitedBGR[0] * gainB;
    splitedBGR[1] = splitedBGR[1] * gainG;
    splitedBGR[2] = splitedBGR[2] * gainR;

    // 将三个单通道图像合成一个三通道图像
    merge(splitedBGR, mDst);

    return 0;
}

int AWB_PerfectReflect(InputArray src, OutputArray dst)
{
    CV_Assert_2(src.channels() == 3, "AWB_PerfectReflect() src image must has 3 channels!");

    Mat mSrc = src.getMat();
    if (mSrc.empty())
    {
        std::cout << "AWB_PerfectReflect() src image can't be empty!" << std::endl;
        return -1;
    }

    dst.create(mSrc.size(), mSrc.type());
    Mat mDst = dst.getMat();

    int sumHist[766] = {0}; // max(R+G+B) = 255*3 = 765, 0~765->766
    int maxVal = 0;

    for (int i = 0; i < mSrc.rows; i++)
    {
        for (int j = 0; j < mSrc.cols; j++)
        {
            Vec3b p = mSrc.at<Vec3b>(i, j);
            int sum = p[0] + p[1] + p[2];
            sumHist[sum]++;
            maxVal = maxVal > p[0] ? maxVal : p[0];
            maxVal = maxVal > p[1] ? maxVal : p[1];
            maxVal = maxVal > p[2] ? maxVal : p[2];
        }
    }

    int totalPixels = 0;
    for (int i = 765; i >= 0; i--)
    {
        totalPixels += sumHist[i];
    }

    CV_Assert_2(totalPixels == mSrc.rows * mSrc.cols, "sumHist pixels number isn't equal with image size!");

    float ratio = 0.1;
    int cumPixel = 0;
    int threshold = 0;
    for (int i = 765; i >= 0; i--)
    {
        cumPixel += sumHist[i];
        if (cumPixel >= ratio * mSrc.rows * mSrc.cols)
        {
            threshold = i;
            break;
        }
    }

    int avgB = 0, avgG = 0, avgR = 0;
    int countPixels = 0;
    for (int i = 0; i < mSrc.rows; i++)
    {
        for (int j = 0; j < mSrc.cols; j++)
        {
            Vec3b p = mSrc.at<Vec3b>(i, j);
            int sum = p[0] + p[1] + p[2];
            if (sum > threshold)
            {
                countPixels++;
                avgB += p[0];
                avgG += p[1];
                avgR += p[2];
            }
        }
    }

    avgB /= countPixels;
    avgG /= countPixels;
    avgR /= countPixels;

    for (int i = 0; i < mSrc.rows; i++)
    {
        for (int j = 0; j < mSrc.cols; j++)
        {
            Vec3b p = mSrc.at<Vec3b>(i, j);
            int B = p[0] * maxVal / avgB;
            B = B > 255 ? 255 : B;
            mDst.at<Vec3b>(i, j)[0] = (uchar)B;

            int G = p[1] * maxVal / avgG;
            G = G > 255 ? 255 : G;
            mDst.at<Vec3b>(i, j)[1] = (uchar)G;

            int R = p[2] * maxVal / avgR;
            R = R > 255 ? 255 : R;
            mDst.at<Vec3b>(i, j)[2] = (uchar)R;
        }
    }

    return 0;
}

int sign(float value)
{
    if (value > 0)
        return 1;
    else if (value == 0)
        return 0;
    else
        return -1;
}

int AWB_DynamicThreshold(InputArray src, OutputArray dst)
{
    CV_Assert(src.channels() == 3);

    Mat mSrc = src.getMat();
    CV_Assert(mSrc.empty() == false);

    dst.create(mSrc.size(), mSrc.type());

    Mat mDst = dst.getMat();
    CV_Assert(mDst.empty() == false);

    // 将RGB图像转换为YCrCb图像
    Mat ycrcb;
    cvtColor(mSrc, ycrcb, COLOR_BGR2YCrCb);
    CV_Assert(ycrcb.empty() == false);

    // 分离YCrCb图像为单通道图像
    std::vector<Mat> splitYCrCb;
    splitYCrCb.reserve(3);
    split(ycrcb, splitYCrCb);

    CV_Assert(splitYCrCb.size() == 3);

    // 将图像分成3x4 12个区域
    std::vector<Mat> splitAreas_Cr;
    splitAreas_Cr.reserve(12);
    std::vector<Mat> splitAreas_Cb;
    splitAreas_Cb.reserve(12);
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int rowStart = i * (mSrc.rows / 3);
            int rowEnd = (i + 1) * (mSrc.rows / 3) - 1;
            int colStart = j * (mSrc.cols / 4);
            int colEnd = (j + 1) * (mSrc.cols / 4) - 1;
            Mat areaCr = splitYCrCb[1](Range(rowStart, rowEnd), Range(colStart, colEnd));
            splitAreas_Cr.push_back(areaCr);

            Mat areaCb = splitYCrCb[2](Range(rowStart, rowEnd), Range(colStart, colEnd));
            splitAreas_Cb.push_back(areaCb);
        }
    }

    CV_Assert(splitAreas_Cr.size() == 12);
    CV_Assert(splitAreas_Cb.size() == 12);

    // 统计每个区域Cr，Cb均值
    float splitAreas_Cr_Mean[12] = {0};
    float splitAreas_Cb_Mean[12] = {0};
    for (int i = 0; i < 12; i++)
    {
        splitAreas_Cb_Mean[i] = mean(splitAreas_Cb[i])[0];
        splitAreas_Cr_Mean[i] = mean(splitAreas_Cr[i])[0];
    }

    // 统计每个区域Cr,Cb偏差值
    float splitAreas_Cr_Std[12] = {0};
    float splitAreas_Cb_Std[12] = {0};
    int splitAreas_Pixels[12] = {0};
    for (int k = 0; k < 12; k++)
    {
        for (int i = 0; i < splitAreas_Cb[k].rows; i++)
        {
            for (int j = 0; j < splitAreas_Cb[k].cols; j++)
            {
                /*splitAreas_Cb_Std[k] += abs(splitAreas_Cb[k].at<uchar>(i, j) - splitAreas_Cb_Mean[k]);
                splitAreas_Cr_Std[k] += abs(splitAreas_Cr[k].at<uchar>(i, j) - splitAreas_Cr_Mean[k]);*/
                splitAreas_Cb_Std[k] += pow(splitAreas_Cb[k].at<uchar>(i, j) - splitAreas_Cb_Mean[k], 2);
                splitAreas_Cr_Std[k] += pow(splitAreas_Cr[k].at<uchar>(i, j) - splitAreas_Cr_Mean[k], 2);
                splitAreas_Pixels[k]++;
            }
        }
    }

    for (int k = 0; k < 12; k++)
    {
        splitAreas_Cb_Std[k] /= splitAreas_Pixels[k];
        splitAreas_Cr_Std[k] /= splitAreas_Pixels[k];
    }

    // 根据每个分块的均值和偏差，计算整个图像的均值和偏差，如果分块的Cb,Cr值过小，则忽略该模块
    float meanCb = 0, meanCr = 0, stdCb = 0, stdCr = 0;
    int areaNum = 0;
    for (int k = 0; k < 12; k++)
    {
        if (splitAreas_Cb_Std[k] > 0.01 && splitAreas_Cr_Std[k] > 0.01)
        {
            areaNum++;
            meanCb += splitAreas_Cb_Mean[k];
            meanCr += splitAreas_Cr_Mean[k];
            stdCb += splitAreas_Cb_Std[k];
            stdCr += splitAreas_Cr_Std[k];
        }
    }

    meanCb /= areaNum;
    meanCr /= areaNum;
    stdCb /= areaNum;
    stdCr /= areaNum;

    // 选择候选白点
    std::vector<Vec2i> yHist[256]; // 记录0-255每一像素值的像素点的坐标 - 符合候选白点条件的像素
    int candinateWhitePixelNum = 0;
    int maxYVal = 0;
    for (int i = 0; i < splitYCrCb[0].rows; i++)
    {
        for (int j = 0; j < splitYCrCb[0].cols; j++)
        {
            bool bCr = std::abs(splitYCrCb[1].at<uchar>(i, j) - (1.5 * meanCr + stdCr /** sign(meanCr)*/)) < 1.5 * stdCr;
            bool bCb = std::abs(splitYCrCb[2].at<uchar>(i, j) - (meanCb + stdCb /** sign(meanCb)*/)) < 1.5 * stdCb;

            int yValue = splitYCrCb[0].at<uchar>(i, j);

            maxYVal = maxYVal > yValue ? maxYVal : yValue;

            if (bCr && bCb)
            {

                yHist[yValue].push_back(Vec2i(i, j));
                candinateWhitePixelNum++;
            }
        }
    }

    int ratio = 0.1; // 获取候选白点中亮度值从高到低前10%作为参考白点
    int cumNum = 0;  // 记录候选白点亮度值从高到低累积像素数
    int yThreshold = 0;
    for (int i = 255; i >= 0; i--)
    {
        cumNum += yHist[i].size();
        if (cumNum > ratio * candinateWhitePixelNum)
        {
            yThreshold = i;
            break;
        }
    }

    // 计算参考白点R,G,B三通道均值
    float avgwR = 0, avgwG = 0, avgwB = 0;

    int whitePixelNum = 0;
    for (int i = 255; i >= yThreshold; i--)
    {
        for (int j = 0; j < yHist[i].size(); j++)
        {
            avgwB += mSrc.at<Vec3b>(yHist[i][j][0], yHist[i][j][1])[0];
            avgwG += mSrc.at<Vec3b>(yHist[i][j][0], yHist[i][j][1])[1];
            avgwR += mSrc.at<Vec3b>(yHist[i][j][0], yHist[i][j][1])[2];
        }
        whitePixelNum += yHist[i].size();
    }

    avgwB /= whitePixelNum;
    avgwG /= whitePixelNum;
    avgwR /= whitePixelNum;

    // 计算增益系数，为了让校正后的图像亮度和原图像亮度一致，计算增益时将Y通道最大值作为参考
    float gainR = maxYVal / avgwR;
    float gainG = maxYVal / avgwG;
    float gainB = maxYVal / avgwB;

    // 矫正图像

    for (int i = 0; i < mSrc.rows; i++)
    {
        for (int j = 0; j < mSrc.cols; j++)
        {
            int B = (int)(mSrc.at<Vec3b>(i, j)[0] * gainB);
            mDst.at<Vec3b>(i, j)[0] = B > 255 ? 255 : B;

            int G = (int)(mSrc.at<Vec3b>(i, j)[1] * gainG);
            mDst.at<Vec3b>(i, j)[1] = G > 255 ? 255 : G;

            int R = (int)(mSrc.at<Vec3b>(i, j)[2] * gainR);
            mDst.at<Vec3b>(i, j)[2] = R > 255 ? 255 : R;
        }
    }

    return 0;
}

int main()
{
    std::string imgPath = "C:\\Temp\\common\\Workspace\\Opencv\\images\\awb_grayworld.jpg";
    Mat src = imread(imgPath);
    Mat dstGW;
    int status = AWB_GrayWorld(src, dstGW);
    if (status != 0)
        goto EXIT;

    imshow("src", src);
    imshow("AWB GrayWorld", dstGW);
    // waitKey(0);

    {
        Mat dstPR;
        status = AWB_PerfectReflect(src, dstPR);
        if (status != 0)
            goto EXIT;

        imshow("AWB PerfectReflect", dstPR);
        // waitKey(0);
    }

    {
        Mat dstDT;
        status = AWB_DynamicThreshold(src, dstDT);
        if (status != 0)
            goto EXIT;

        imshow("AWB DynamicThreshold", dstDT);
        waitKey(0);
    }

EXIT:
    system("pause");
    destroyAllWindows();

    return 0;
}
