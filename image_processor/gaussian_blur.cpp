#include "gaussian_blur.h"

GaussianBlur::GaussianBlur(const BlurParams &blur_params) : blur_params_(blur_params) {
}

std::unique_ptr<GaussianBlur> GaussianBlur::CreateGaussianBlur(const BlurParams &blur_params) {
    return std::unique_ptr<GaussianBlur>(new GaussianBlur(blur_params));
}

void GaussianBlur::Apply(BMPInfo *bmp_info) {
    std::vector<double> gaussian_const(std::max(bmp_info->width, bmp_info->height));
    for (size_t i = 0; i < std::max(bmp_info->width, bmp_info->height); ++i) {
        gaussian_const[i] = std::pow(M_E, -(std::pow(i, 2)) / (2 * std::pow(blur_params_.sigma, 2))) /
                            (std::sqrt(2 * M_PI * std::pow(blur_params_.sigma, 2)));
    }
    PartCountBlurSum(bmp_info->image, gaussian_const, bmp_info->height, bmp_info->width, bmp_info->width, false);
    PartCountBlurSum(bmp_info->image, gaussian_const, bmp_info->height, bmp_info->width, bmp_info->height, true);
}

void GaussianBlur::PartCountBlurSum(Image &new_image, const std::vector<double> &consts, int height, int width,
                                    int const_in_degree, bool checker) {
    Image changed_image(height);
    for (int i = 0; i < height; ++i) {
        changed_image[i].resize(width);
        for (int j = 0; j < width; ++j) {
            PixelColor new_pixel_color;
            for (int const_param = 0; const_param < const_in_degree; ++const_param) {
                if (checker) {
                    new_pixel_color.blue += new_image[const_param][j].blue * consts[std::abs(i - const_param)];
                    new_pixel_color.green += new_image[const_param][j].green * consts[std::abs(i - const_param)];
                    new_pixel_color.red += new_image[const_param][j].red * consts[std::abs(i - const_param)];
                } else {
                    new_pixel_color.blue += new_image[i][const_param].blue * consts[std::abs(j - const_param)];
                    new_pixel_color.green += new_image[i][const_param].green * consts[std::abs(j - const_param)];
                    new_pixel_color.red += new_image[i][const_param].red * consts[std::abs(j - const_param)];
                }
                if (new_pixel_color.blue >= 1. || new_pixel_color.green >= 1. || new_pixel_color.red >= 1.) {
                    new_pixel_color.blue = std::min(new_pixel_color.blue, 1.);
                    new_pixel_color.green = std::min(new_pixel_color.green, 1.);
                    new_pixel_color.red = std::min(new_pixel_color.red, 1.);
                    break;
                }
            }
            changed_image[i][j] = new_pixel_color;
        }
    }
    new_image = changed_image;
}
