#include <unordered_map>
#include <variant>
#include <string_view>
#include <vector>
#include <string>


using Argument = std::variant<std::string *, float *, size_t *>;
using ArgWithName = std::pair<std::string, Argument>;

struct Internal {
    Internal(const char *data);

    void operator()(std::string *s);

    void operator()(size_t *s);

    void operator()(float *s);

private:
    const char *data_;
};

class Parser {
public:
    Parser() = default;

    void AddPositionalArg(const std::string &arg_name, Argument output);

    void AddKeywordArg(const std::string &keyword, const std::vector<ArgWithName> args);

    void Parse(size_t argc, char **argv);

    std::vector<std::string_view> GetFilters();


private:
    std::vector<ArgWithName> positional_args_;
    std::unordered_map<std::string, std::vector<ArgWithName>> keyword_fields_;
    std::vector<std::string_view> filters_order_;
};
