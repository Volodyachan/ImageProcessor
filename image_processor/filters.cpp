#include "filters.h"

void Factory::Register(std::string_view name, std::function<std::unique_ptr<AllFilters>()> creator) {
    name_to_func[name] = creator;
}

std::unique_ptr<AllFilters> Factory::Create(std::string_view name) {
    return name_to_func[name]();
}

MatrixFilter::MatrixFilter(const std::vector<std::vector<double>> &matrix) : matrix_(matrix) {
}

void MatrixFilter::Apply(BMPInfo *bmp_info) {
    Image new_image;
    new_image.resize(bmp_info->height);
    for (int i = 0; i < static_cast<int>(bmp_info->height); ++i) {
        new_image[i].resize(bmp_info->width);
    }
    for (int i = 0; i < static_cast<int>(bmp_info->height); ++i) {
        for (int j = 0; j < static_cast<int>(bmp_info->width); ++j) {
            PixelColor new_pixel_color;
            for (int x = -1; x <= 1; ++x) {
                for (int y = -1; y <= 1; ++y) {
                    if (0 <= i + y and i + y < static_cast<int>(bmp_info->height) and 0 <= j + x and
                        j + x < static_cast<int>(bmp_info->width)) {
                        new_pixel_color.blue += (bmp_info->image[i + y][j + x].blue * matrix_[y + 1][x + 1]);
                        new_pixel_color.green += (bmp_info->image[i + y][j + x].green * matrix_[y + 1][x + 1]);
                        new_pixel_color.red += (bmp_info->image[i + y][j + x].red * matrix_[y + 1][x + 1]);
                    } else {
                        int nearest_x = 0;
                        int nearest_y = 0;
                        if (i + y < 0) {
                            nearest_y = i + y + 2;
                        } else if (i + y >= static_cast<int>(bmp_info->height)) {
                            nearest_y = i + y - 2;
                        }
                        if (j + x < 0) {
                            nearest_x = j + x + 2;
                        } else if (j + x >= static_cast<int>(bmp_info->width)) {
                            nearest_x = j + x - 2;
                        }
                        new_pixel_color.blue += (bmp_info->image[nearest_y][nearest_x].blue) * matrix_[y + 1][x + 1];
                        new_pixel_color.green += (bmp_info->image[nearest_y][nearest_x].green) * matrix_[y + 1][x + 1];
                        new_pixel_color.red += (bmp_info->image[nearest_y][nearest_x].red) * matrix_[y + 1][x + 1];
                    }
                }
            }
            new_image[i][j].blue = std::min(1.0, std::max(0.0, new_pixel_color.blue));
            new_image[i][j].green = std::min(1.0, std::max(0.0, new_pixel_color.green));
            new_image[i][j].red = std::min(1.0, std::max(0.0, new_pixel_color.red));
        }
    }
    bmp_info->image = new_image;
}