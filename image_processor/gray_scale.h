#pragma once

#include "filters.h"

class Grayscale : public AllFilters {
public:
    Grayscale() = default;

    static std::unique_ptr<Grayscale> CreateGrayscale();

    void Apply(BMPInfo *bmp_info) override;

    static void MakeGrayscale(BMPInfo *bmp_info);
};
