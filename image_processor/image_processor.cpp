#include "Parser.h"
#include "bmp_reader.h"
#include "filters.h"
#include "crop.h"
#include "gray_scale.h"
#include "negative.h"
#include "sharpening.h"
#include "edge_detection.h"
#include "gaussian_blur.h"
#include "bilateral.h"
#include "exceptions.h"
#include "iostream"


int main(int argc, char **argv) {
    Parser parser;
    BMPReader bmp_file;

    std::string input_file;
    std::string output_file;

    CropParams crop_params;
    BlurParams blur_params;
    EdgeParams edge_params;
    BilateralParams bilateral_params;

    BMPInfo bmp_info;

    Factory factory;

    parser.AddPositionalArg("input_file", &input_file);
    parser.AddPositionalArg("output_file", &output_file);
    parser.AddKeywordArg("-crop", {{"width",  &crop_params.width},
                                   {"height", &crop_params.height}});
    parser.AddKeywordArg("-blur", {{"sigma", &blur_params.sigma}});
    parser.AddKeywordArg("-gs", {});
    parser.AddKeywordArg("-neg", {});
    parser.AddKeywordArg("-sharp", {});
    parser.AddKeywordArg("-edge", {{"threshold", &edge_params.threshold}});
    parser.AddKeywordArg("-bil", {{"diameter", &bilateral_params.diameter},
                                  {"sigmaI",   &bilateral_params.sigmaI},
                                  {"sigmaS",   &bilateral_params.sigmaS}});

    try {
        parser.Parse(argc, argv);
    } catch (Exception &ex) {
        std::cout << Exceptions::ExceptionValue(ex) << std::endl;
        return 0;
    }

    bmp_file.Read(input_file, bmp_info);

    auto NewCreateCrop = std::bind(Crop::CreateCrop, crop_params);
    factory.Register("-crop", NewCreateCrop);

    factory.Register("-gs", Grayscale::CreateGrayscale);

    factory.Register("-neg", Negative::CreateNegative);

    factory.Register("-sharp", Sharpening::CreateSharpening);

    auto NewCreateEdgeDetection = std::bind(EdgeDetection::CreateEdgeDetection, edge_params);
    factory.Register("-edge", NewCreateEdgeDetection);

    auto NewCreateGaussianBlur = std::bind(GaussianBlur::CreateGaussianBlur, blur_params);
    factory.Register("-blur", NewCreateGaussianBlur);

    auto NewCreateBilateral = std::bind(Bilateral::CreateBilateral, bilateral_params);
    factory.Register("-bil", NewCreateBilateral);

    for (auto filter: parser.GetFilters()) {
        std::unique_ptr<AllFilters> obj = factory.Create(filter);
        obj->Apply(&bmp_info);
    }

    bmp_file.Write(output_file, bmp_info);
    return 0;
}
