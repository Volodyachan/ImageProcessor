#include "filters.h"
#include "gray_scale.h"
#include "sharpening.h"


struct EdgeParams {
    float threshold;
};

class EdgeDetection : public MatrixFilter {
public:
    EdgeDetection(const EdgeParams &edge_params);

    static std::unique_ptr<EdgeDetection> CreateEdgeDetection(const EdgeParams &edge_params);

    void Apply(BMPInfo *bmp_info) override;

private:
    EdgeParams edge_params_;
};
