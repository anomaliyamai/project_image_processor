#pragma once
#include <stdexcept>
#include <string_view>
#include <vector>

struct FilterDescriptor {
    std::string_view filter_name;
    std::vector<std::string_view> filter_params;
};

class Parser {
public:
    Parser(int argc, char* argv[]);
    std::string_view GetInputFileName() const;
    std::string_view GetOutputFileName() const;
    std::vector<FilterDescriptor*> GetVectorOfFilterDescriptor() {
        return storing_filters_and_their_params_;
    };

private:
    std::vector<FilterDescriptor*> storing_filters_and_their_params_;
    std::string_view input_file_name_;
    std::string_view output_file_name_;
};