#include "edge_detection.h"

EdgeDetection::EdgeDetection(const EdgeParams &edge_params) : MatrixFilter({{0.,  -1., 0.},
                                                                            {-1., 4.,  -1},
                                                                            {0.,  -1., 0.}}),
                                                              edge_params_(edge_params) {
}

std::unique_ptr<EdgeDetection> EdgeDetection::CreateEdgeDetection(const EdgeParams &edge_params) {
    return std::unique_ptr<EdgeDetection>(new EdgeDetection(edge_params));
}

void EdgeDetection::Apply(BMPInfo *bmp_info) {
    Grayscale::MakeGrayscale(bmp_info);
    MatrixFilter::Apply(bmp_info);

    for (size_t i = 0; i < bmp_info->height; ++i) {
        for (size_t j = 0; j < bmp_info->width; ++j) {
            if (bmp_info->image[i][j].blue > static_cast<double>(edge_params_.threshold)) {
                bmp_info->image[i][j].blue = 1.;
                bmp_info->image[i][j].green = 1.;
                bmp_info->image[i][j].red = 1.;
            } else {
                bmp_info->image[i][j].blue = 0.;
                bmp_info->image[i][j].green = 0.;
                bmp_info->image[i][j].red = 0.;
            }
        }
    }
}
