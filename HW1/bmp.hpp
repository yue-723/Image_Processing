#ifndef BMP_HPP
#define BMP_HPP

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <sys/stat.h>
using namespace std;

// BMP File Header -> 14 bytes
typedef struct __attribute__((__packed__))
{
    uint16_t fileMarker;
    unsigned int bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    unsigned int bfOffset;
} FILEHEADER;

// BMP Info Header -> 40 bytes
typedef struct __attribute__((__packed__))
{
    unsigned int biSize;
    int width;
    int height;
    uint16_t planes;
    uint16_t bitPix;
    unsigned int biCompression;
    unsigned int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
} INFOHEADER;

// RGB color -> 3bytes of pixel depth
typedef struct
{
    uint8_t B;
    uint8_t G;
    uint8_t R;
} RGB;

// ARGB color -> 4bytes of pixel depth
typedef struct
{
    uint8_t B;
    uint8_t G;
    uint8_t R;
    uint8_t A;
} ARGB;

// BMP class -> io processing and bmp data processing
class BMP
{
private:
    const char *IMAGE_PATH;
    FILEHEADER header;
    INFOHEADER info;

public:
    bool isRGB;
    int IMG_width;
    int IMG_height;
    vector<vector<RGB>> RGB_color;
    vector<vector<ARGB>> ARGB_color;

    BMP(const char *path) : IMAGE_PATH(path) { Parse(); }
    ~BMP()
    {
        if (isRGB)
        {
            for (int i = 0; i < info.height; i++)
            {
                RGB_color[i].clear();
                RGB_color[i].shrink_to_fit();
            }

            RGB_color.clear();
            RGB_color.shrink_to_fit();
        }
        else
        {
            for (int i = 0; i < info.height; i++)
            {
                ARGB_color[i].clear();
                ARGB_color[i].shrink_to_fit();
            }
            ARGB_color.clear();
            ARGB_color.shrink_to_fit();
        }
    }

    void Parse() // parsing *.bmp to get FILEHEADER, INFOHEADER, Bitmap Array(Color)
    {
        // read image
        ifstream IMG(IMAGE_PATH, ios::in | ios::binary);
        if (IMG.fail())
        {
            cout << "Loading failed\n";
            exit(1);
        }
        IMG.read(reinterpret_cast<char *>(&header), sizeof(FILEHEADER)); // get FILEHEADER
        IMG.read(reinterpret_cast<char *>(&info), sizeof(INFOHEADER));   // get INFOHEADER

        IMG_width = info.width;
        IMG_height = info.height;
        if (info.bitPix == 24) // check pixel depth and get Bitmap Array(Color)
        {
            isRGB = true;
            vector<RGB> buffer;
            for (int i = 0; i < info.height; i++)
            {
                for (int j = 0; j < info.width; j++)
                {
                    RGB rgb;
                    IMG.read((char *)&rgb, sizeof(RGB));
                    buffer.push_back(rgb);
                }
                RGB_color.push_back(buffer);
                buffer.clear();
            }
        }
        else
        {
            isRGB = false;
            vector<ARGB> buffer;
            for (int i = 0; i < info.height; i++)
            {
                for (int j = 0; j < info.width; j++)
                {
                    ARGB argb;
                    IMG.read((char *)&argb, sizeof(ARGB));
                    buffer.push_back(argb);
                }
                ARGB_color.push_back(buffer);
                buffer.clear();
            }
        }
        IMG.close();
    }
    void Save(const char *Save_path) // save *.bmp
    {
        struct stat sb;
        if (stat("output_bmp", &sb) != 0)
            mkdir("./output_bmp", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

        ofstream OUTPUT_IMG(Save_path, ios::out | ios::binary);
        if (IMG_width != info.width) // for scaling
        {
            info.width = IMG_width;
            info.height = IMG_height;
            info.biSizeImage = IMG_width * IMG_height * info.bitPix / 8;
            header.bfSize = info.biSizeImage + 54;
        }
        OUTPUT_IMG.write(reinterpret_cast<char *>(&header), sizeof(FILEHEADER));
        OUTPUT_IMG.write(reinterpret_cast<char *>(&info), sizeof(INFOHEADER));
        if (isRGB)
            for (int i = 0; i < info.height; i++)
                for (int j = 0; j < info.width; j++)
                {
                    RGB rgb = RGB_color[i][j];
                    OUTPUT_IMG.write((char *)&rgb, sizeof(RGB));
                }
        else
            for (int i = 0; i < info.height; i++)
                for (int j = 0; j < info.width; j++)
                {
                    ARGB argb = ARGB_color[i][j];
                    OUTPUT_IMG.write((char *)&argb, sizeof(ARGB));
                }
        OUTPUT_IMG.close();
    }
};

#endif