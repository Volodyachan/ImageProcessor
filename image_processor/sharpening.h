#pragma once

#include "filters.h"

class Sharpening : public MatrixFilter {
public:
    Sharpening();

    static std::unique_ptr<Sharpening> CreateSharpening();

    void Apply(BMPInfo *bmp_info) override;
};
