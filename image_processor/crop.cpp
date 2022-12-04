#include "crop.h"

Crop::Crop(const CropParams &params) : params_(params) {
}

std::unique_ptr<Crop> Crop::CreateCrop(const CropParams &params) {
    return std::unique_ptr<Crop>(new Crop(params));
}

void Crop::Apply(BMPInfo *bmp_info) {
    if (params_.width < bmp_info->width) {
        bmp_info->width = params_.width;
        bmp_info->DIB_header[4] = static_cast<unsigned char>(params_.width & 0xFF);
        bmp_info->DIB_header[5] = static_cast<unsigned char>((params_.width >> 8) & 0xFF);
        bmp_info->DIB_header[6] = static_cast<unsigned char>((params_.width >> 16) & 0xFF);
        bmp_info->DIB_header[7] = static_cast<unsigned char>((params_.width >> 24) & 0xFF);
    }

    if (params_.height < bmp_info->height) {
        int j = 0;
        for (size_t i = bmp_info->height - params_.height; i < bmp_info->height; ++i) {
            std::swap(bmp_info->image[i], bmp_info->image[j]);
            ++j;
        }
        bmp_info->height = params_.height;

        bmp_info->DIB_header[8] = static_cast<unsigned char>(params_.height & 0xFF);
        bmp_info->DIB_header[9] = static_cast<unsigned char>((params_.height >> 8) & 0xFF);
        bmp_info->DIB_header[10] = static_cast<unsigned char>((params_.height >> 16) & 0xFF);
        bmp_info->DIB_header[11] = static_cast<unsigned char>((params_.height >> 24) & 0xFF);
    }

    int img_size = 14 + 40 + (bmp_info->width * 3 + (4 - ((bmp_info->width * 3) % 4)) % 4) * bmp_info->height;
    bmp_info->bmp_header[2] = static_cast<unsigned char>(img_size & 0xFF);
    bmp_info->bmp_header[3] = static_cast<unsigned char>((img_size >> 8) & 0xFF);
    bmp_info->bmp_header[4] = static_cast<unsigned char>((img_size >> 16) & 0xFF);
    bmp_info->bmp_header[5] = static_cast<unsigned char>((img_size >> 24) & 0xFF);

}
