enum class Exception {
    IncorrectPositionalArguments,
    IncorrectNameOfFilter,
    IncorrectAmountOfArguments,
    IncorrectInputFile,
    IncorrectTypeOfArgument,
};

class Exceptions {
public:
    static std::string ExceptionValue(const Exception &value) {
        if (value == Exception::IncorrectPositionalArguments) {
            return "Incorrect input or output file";
        } else if (value == Exception::IncorrectNameOfFilter) {
            return "Invalid param of filter";
        } else if (value == Exception::IncorrectAmountOfArguments) {
            return "Incorrect amount of arguments";
        } else if (value == Exception::IncorrectTypeOfArgument) {
            return "Incorrect type of argument";
        } else {
            return "Incorrect input file";
        }
    }
};
