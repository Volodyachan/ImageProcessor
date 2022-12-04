#include "filters.h"

struct BlurParams {
    float sigma;
};

class GaussianBlur : public AllFilters {
public:
    GaussianBlur(const BlurParams &blur_params);

    static std::unique_ptr<GaussianBlur> CreateGaussianBlur(const BlurParams &blur_params);

    void Apply(BMPInfo *bmp_info) override;

    void
    PartCountBlurSum(Image &new_image, const std::vector<double> &consts, int height, int width, int const_in_degree,
                     bool checker);

private:
    BlurParams blur_params_;
};

