#include "filters.h"


struct BilateralParams {
    size_t diameter;
    float sigmaI;
    float sigmaS;
};

class Bilateral : public AllFilters {
public:
    Bilateral(const BilateralParams &bilateral_params);

    static std::unique_ptr<Bilateral> CreateBilateral(const BilateralParams &bilateral_params);

    void Apply(BMPInfo *bmp_info) override;

    void PartCountBilateralSum(Image &new_image, const std::vector<double> &consts, int height, int width,
                               bool checker);

    double PixelIntensity(const double &first_pixel, const double &second_pixel) const;

private:
    BilateralParams bilateral_params_;
};
