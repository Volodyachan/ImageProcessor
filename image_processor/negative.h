#include "filters.h"

class Negative : public AllFilters {
public:
    Negative() = default;

    static std::unique_ptr<Negative> CreateNegative();

    void Apply(BMPInfo *bmp_info) override;
};
