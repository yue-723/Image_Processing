#ifndef BMP_HPP
#define BMP_HPP

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <sys/stat.h>
using namespace std;

typedef struct __attribute__((__packed__)){
    unsigned char fileMarker1;
    unsigned char fileMarker2;
    unsigned int bfSize;
    uint16_t unused1;
    uint16_t unused2;
    unsigned int imageDataOffset;
} FILEHEADER;

typedef struct __attribute__((__packed__)){
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

typedef struct
{
    uint8_t B;
    uint8_t G;
    uint8_t R;
} RGB;

typedef struct{
    uint8_t B;
    uint8_t G;
    uint8_t R;
    uint8_t A;
} ARGB;

class BMP{
private:
    const char* IMAGE_PATH;
    FILEHEADER header;
    INFOHEADER info;
    RGB** RGB_color;
    ARGB** ARGB_color;
public:
    BMP(const char* path) : IMAGE_PATH(path), RGB_color(NULL), ARGB_color(NULL) { parse(); }
    ~BMP()
    {
        if (RGB_color)
        {
            for (int i = 0; i < info.width; i++)
                delete[] RGB_color[i];
            delete[] RGB_color;
        }
        else
        {
            for (int i = 0; i < info.height; i++)
                delete[] ARGB_color[i];
            delete[] ARGB_color;
        }
    }

    void parse()
    {
        FILE* IMG = fopen(IMAGE_PATH, "rb");
        if (IMG == NULL)
        {
            cout << "Loading failed\n";
            exit(1);
        }
        fread(&header, sizeof(unsigned char), sizeof(FILEHEADER), IMG);
        fread(&info, sizeof(unsigned char), sizeof(INFOHEADER), IMG);

        if (info.bitPix == 24)
        {
            RGB_color = new RGB* [info.height];
            for (int i = 0; i < info.height; i++)
                RGB_color[i] = new RGB[info.width];
            for (int i = 0; i < info.height; i++)
                for (int j = 0; j < info.width; j++)
                {
                    RGB rgb;
                    fread(&rgb, sizeof(RGB), 1, IMG);
                    RGB_color[i][j] = rgb;
                }
        }
        else
        {
            ARGB_color = new ARGB* [info.height];
            for (int i = 0; i < info.height; i++)
                ARGB_color[i] = new ARGB[info.width];
            for (int i = 0; i < info.height; i++)
                for (int j = 0; j < info.width; j++)
                {
                    ARGB argb;
                    fread(&argb, sizeof(RGB), 1, IMG);
                    ARGB_color[i][j] = argb;
                }
        }
        fclose(IMG);
    }
    void Export(const char* Save_path)
    {
        struct stat sb;
        if (stat("output_bmp", &sb) != 0)
            mkdir("./output_bmp", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            
        FILE* OUTPUT = fopen(Save_path, "wb");
        fwrite(&header, sizeof(unsigned char), sizeof(FILEHEADER), OUTPUT);
        fwrite(&info, sizeof(unsigned char), sizeof(INFOHEADER), OUTPUT);

        for (int i = 0; i < info.height; i++)
            for (int j = 0; j < info.width; j++)
            {
                RGB rgb = RGB_color[i][j];
                fwrite(&rgb, sizeof(RGB), 1, OUTPUT);
            }
        fclose(OUTPUT);
    }
};

#endif