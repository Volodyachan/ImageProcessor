#include "Parser.h"
#include "exceptions.h"


Internal::Internal(const char *data) : data_(data) {
}

void Internal::operator()(std::string *s) {
    *s = std::string(data_);
}

void Internal::operator()(size_t *s) {
    *s = std::stoi(data_);
}

void Internal::operator()(float *s) {
    *s = std::stof(data_);
}

void Parser::AddPositionalArg(const std::string &arg_name, Argument output) {
    positional_args_.push_back({arg_name, output});
}

void Parser::AddKeywordArg(const std::string &keyword, const std::vector<ArgWithName> args) {
    keyword_fields_[keyword] = args;
}

void Parser::Parse(size_t argc, char **argv) {
    if (argc < 3) {
        throw Exception::IncorrectPositionalArguments;
    }
    for (size_t i = 0; i < positional_args_.size(); ++i) {
        Internal assign(argv[i + 1]);
        std::visit(assign, positional_args_[i].second);
        auto size = (get<std::string *>(positional_args_[i].second))->size();
        if (size < 5 or get<std::string *>(positional_args_[i].second)->substr(size - 4, size) != ".bmp") {
            throw Exception::IncorrectPositionalArguments;
        }
    }
    for (size_t i = positional_args_.size() + 1; i < argc; ++i) {
        auto it = keyword_fields_.find(argv[i]);
        if (it == keyword_fields_.end()) {
            throw Exception::IncorrectNameOfFilter;
        }
        if (std::find(filters_order_.begin(), filters_order_.end(), argv[i]) != filters_order_.end()) {
            throw Exception::IncorrectNameOfFilter;
        }
        filters_order_.push_back(argv[i]);
        auto param_required = it->second.size();
        for (size_t j = 1; j <= param_required; ++j) {
            if (i + j >= argc) {
                throw Exception::IncorrectAmountOfArguments;
            }
            Internal assign(argv[i + j]);
            try {
                std::visit(assign, it->second[j - 1].second);
            } catch (std::invalid_argument) {
                throw Exception::IncorrectTypeOfArgument;
            }
        }
        if (i + param_required + 1 < argc &&
            keyword_fields_.find(argv[i + param_required + 1]) == keyword_fields_.end()) {
            throw Exception::IncorrectAmountOfArguments;
        }
        i += param_required;
    }
}

std::vector<std::string_view> Parser::GetFilters() {
    return filters_order_;
}
