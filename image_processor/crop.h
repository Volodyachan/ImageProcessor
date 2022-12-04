#include "filters.h"


struct CropParams {
    size_t width;
    size_t height;
};

class Crop : public AllFilters {
public:
    Crop(const CropParams &params);

    static std::unique_ptr<Crop> CreateCrop(const CropParams &params);

    void Apply(BMPInfo *bmp_info) override;

private:
    CropParams params_;
};
