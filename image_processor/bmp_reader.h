#pragma once

#include <vector>
#include <string>


static const int MAX_PIXEL_VALUE = 255;

struct PixelColor {
    double blue = 0.;
    double green = 0.;
    double red = 0.;
};

using Image = std::vector<std::vector<PixelColor>>;

struct BMPInfo {
    unsigned char bmp_header[14];
    unsigned char DIB_header[40];
    uint32_t width = 0;
    uint32_t height = 0;
    Image image;
};

class BMPReader {
public:
    BMPReader() = default;

    void Read(const std::string &filename, BMPInfo &bmp_info);

    void Write(const std::string &filename, BMPInfo &bmp_info);
};
