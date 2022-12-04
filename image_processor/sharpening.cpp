#include "sharpening.h"


std::unique_ptr<Sharpening> Sharpening::CreateSharpening() {
    return std::unique_ptr<Sharpening>(new Sharpening());
}

void Sharpening::Apply(BMPInfo *bmp_info) {
    MatrixFilter::Apply(bmp_info);
}

Sharpening::Sharpening() : MatrixFilter({{0.,  -1., 0.},
                                         {-1., 5.,  -1.},
                                         {0.,  -1., 0.}}) {
}

