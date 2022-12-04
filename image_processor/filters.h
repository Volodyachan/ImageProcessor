#pragma once

#include "bmp_reader.h"
#include <unordered_map>
#include <string_view>

class AllFilters {
public:
    virtual ~AllFilters() = default;

    virtual void Apply(BMPInfo *bmp_info) = 0;
};

class Factory {
public:
    void Register(std::string_view name, std::function<std::unique_ptr<AllFilters>()> creator);

    std::unique_ptr<AllFilters> Create(std::string_view name);

private:
    std::unordered_map<std::string_view, std::function<std::unique_ptr<AllFilters>()>> name_to_func;
};

class MatrixFilter : public AllFilters {
public:
    MatrixFilter(const std::vector<std::vector<double>> &matrix);

    void Apply(BMPInfo *bmp_info) override;

private:
    std::vector<std::vector<double>> matrix_;
};
