#include "gray_scale.h"

std::unique_ptr<Grayscale> Grayscale::CreateGrayscale() {
    return std::unique_ptr<Grayscale>(new Grayscale());
}

void Grayscale::Apply(BMPInfo *bmp_info) {
    MakeGrayscale(bmp_info);
}

void Grayscale::MakeGrayscale(BMPInfo *bmp_info) {
    for (size_t i = 0; i < bmp_info->height; ++i) {
        for (size_t j = 0; j < bmp_info->width; ++j) {
            auto new_color = 0.299 * bmp_info->image[i][j].red + 0.587 * bmp_info->image[i][j].green +
                             0.114 * bmp_info->image[i][j].blue;
            bmp_info->image[i][j].blue = std::min(new_color, 1.);
            bmp_info->image[i][j].green = std::min(new_color, 1.);
            bmp_info->image[i][j].red = std::min(new_color, 1.);
        }
    }
}
