#include "negative.h"

std::unique_ptr<Negative> Negative::CreateNegative() {
    return std::unique_ptr<Negative>(new Negative());
}

void Negative::Apply(BMPInfo *bmp_info) {
    for (size_t i = 0; i < bmp_info->height; ++i) {
        for (size_t j = 0; j < bmp_info->width; ++j) {
            bmp_info->image[i][j].red = 1 - bmp_info->image[i][j].red;
            bmp_info->image[i][j].green = 1 - bmp_info->image[i][j].green;
            bmp_info->image[i][j].blue = 1 - bmp_info->image[i][j].blue;
        }
    }
}
