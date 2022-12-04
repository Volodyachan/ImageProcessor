#include "bilateral.h"

Bilateral::Bilateral(const BilateralParams &bilateral_params) : bilateral_params_(bilateral_params) {
}

std::unique_ptr<Bilateral> Bilateral::CreateBilateral(const BilateralParams &bilateral_params) {
    return std::unique_ptr<Bilateral>(new Bilateral(bilateral_params));
}

void Bilateral::Apply(BMPInfo *bmp_info) {
    std::vector<double> gaussian_consts(std::max(bmp_info->width, bmp_info->height));
    for (size_t i = 0; i < std::max(bmp_info->width, bmp_info->height); ++i) {
        gaussian_consts[i] = std::pow(M_E, -(std::pow(i, 2)) / (2 * std::pow(bilateral_params_.sigmaI, 2))) /
                            (std::sqrt(2 * M_PI * std::pow(bilateral_params_.sigmaI, 2)));
    }
    PartCountBilateralSum(bmp_info->image, gaussian_consts, bmp_info->height, bmp_info->width,
                          true);
    PartCountBilateralSum(bmp_info->image, gaussian_consts, bmp_info->height, bmp_info->width,
                          false);
}

void
Bilateral::PartCountBilateralSum(Image &new_image, const std::vector<double> &consts, int height, int width,
                                 bool checker) {
    Image changed_image(height);
    int radius = bilateral_params_.diameter / 2;
    for (int i = radius; i < height - radius; ++i) {
        changed_image[i].resize(width);
        for (int j = radius; j < width - radius; ++j) {
            PixelColor new_pixel_color;
            double wp = 0.;
            if (checker) {
                for (int x = i - radius; x < i + radius; ++x) {
                    if (0 <= x && x < height) {
                        double intensity = PixelIntensity(
                                new_image[i][j].blue + new_image[i][j].green + new_image[i][j].red,
                                new_image[x][j].blue + new_image[x][j].green + new_image[x][j].red);
                        new_pixel_color.blue += new_image[x][j].blue * consts[std::abs(i - x)] * intensity;
                        new_pixel_color.green += new_image[x][j].green * consts[std::abs(i - x)] * intensity;
                        new_pixel_color.red += new_image[x][j].red * consts[std::abs(i - x)] * intensity;

                        wp += consts[std::abs(i - x)] * intensity;
                    }
                }
            } else {
                for (int y = j - radius; y < j + radius; ++y) {
                    if (0 <= y && y < width) {
                        double intensity = PixelIntensity(
                                new_image[i][j].blue + new_image[i][j].green + new_image[i][j].red,
                                new_image[i][y].blue + new_image[i][y].green + new_image[i][y].red);
                        new_pixel_color.blue += new_image[i][y].blue * consts[std::abs(j - y)] * intensity;
                        new_pixel_color.green += new_image[i][y].green * consts[std::abs(j - y)] * intensity;
                        new_pixel_color.red += new_image[i][y].red * consts[std::abs(j - y)] * intensity;

                        wp += consts[std::abs(j - y)] * intensity;
                    }
                }
            }

            changed_image[i][j].blue = std::min(new_pixel_color.blue / wp, 1.);
            changed_image[i][j].green = std::min(new_pixel_color.green / wp, 1.);
            changed_image[i][j].red = std::min(new_pixel_color.red / wp, 1.);
        }
    }
    for (size_t i = bilateral_params_.diameter / 2; i < height - bilateral_params_.diameter / 2; ++i) {
        for (size_t j = bilateral_params_.diameter / 2; j < width - bilateral_params_.diameter / 2; ++j) {
            new_image[i][j] = changed_image[i][j];
        }
    }
}

double Bilateral::PixelIntensity(const double &first_pixel, const double &second_pixel) const {
    return std::pow(M_E, -(std::pow((first_pixel - second_pixel) * MAX_PIXEL_VALUE, 2) /
                           (2 * std::pow(bilateral_params_.sigmaS, 2)))) /
           (std::sqrt(2 * M_PI * std::pow(bilateral_params_.sigmaS, 2)));
}
