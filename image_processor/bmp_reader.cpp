#include "bmp_reader.h"
#include <fstream>
#include "exceptions.h"


void BMPReader::Read(const std::string &filename, BMPInfo &bmp_info) {
    std::ifstream file_stream;
    file_stream.open(filename, std::ios::in | std::ios::binary);
    if (!file_stream.is_open()) {
        throw Exception::IncorrectInputFile;
    }
    file_stream.read(reinterpret_cast<char *>(bmp_info.bmp_header), 14);
    file_stream.read(reinterpret_cast<char *>(bmp_info.DIB_header), 40);
    bmp_info.width = bmp_info.DIB_header[4] + (bmp_info.DIB_header[5] << 8) + (bmp_info.DIB_header[6] << 16) +
                     (bmp_info.DIB_header[7] << 24);
    bmp_info.height = bmp_info.DIB_header[8] + (bmp_info.DIB_header[9] << 8) + (bmp_info.DIB_header[10] << 16) +
                      (bmp_info.DIB_header[11] << 24);

    int pud_num = ((4 - (bmp_info.width * 3) % 4) % 4);

    bmp_info.image.resize(bmp_info.height);
    for (size_t i = 0; i < bmp_info.height; i++) {
        bmp_info.image[i].resize(bmp_info.width);
        for (size_t j = 0; j < bmp_info.width; j++) {
            unsigned char color[3];
            file_stream.read(reinterpret_cast<char *>(color), 3);
            bmp_info.image[i][j] = {static_cast<double>(color[0]) / MAX_PIXEL_VALUE,
                                    static_cast<double >(color[1]) / MAX_PIXEL_VALUE,
                                    static_cast<double >(color[2]) / MAX_PIXEL_VALUE};
        }
        file_stream.ignore(pud_num);
    }
    file_stream.close();
}

void BMPReader::Write(const std::string &filename, BMPInfo &bmp_info) {
    std::ofstream file_stream;
    file_stream.open(filename, std::ios::out | std::ios::binary);
    file_stream.write(reinterpret_cast<char *>(bmp_info.bmp_header), 14);
    file_stream.write(reinterpret_cast<char *>(bmp_info.DIB_header), 40);
    int pud_num = ((4 - (bmp_info.width * 3) % 4) % 4);
    unsigned char pad[] = {0};
    for (size_t i = 0; i < bmp_info.height; i++) {
        for (size_t j = 0; j < bmp_info.width; j++) {
            unsigned char color[3] = {static_cast<unsigned char>(bmp_info.image[i][j].blue * MAX_PIXEL_VALUE),
                                      static_cast<unsigned char>(bmp_info.image[i][j].green * MAX_PIXEL_VALUE),
                                      static_cast<unsigned char>(bmp_info.image[i][j].red * MAX_PIXEL_VALUE)};
            file_stream.write(reinterpret_cast<char *>(color), 3);
        }
        file_stream.write(reinterpret_cast<char *>(pad), pud_num);
    }
    file_stream.close();
}

