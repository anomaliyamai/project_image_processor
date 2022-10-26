#include "parser.h"

Parser::Parser(int argc, char* argv[]) {
    static const int MIN_PARAM_NUM = 3;
    static const int INPUT_FILE_NAME_POS = 1;
    static const int OUTPUT_FILE_NAME_POS = 2;
    if (argc < MIN_PARAM_NUM) {
        throw std::invalid_argument("Not enough parameters for parsing");
    }
    input_file_name_ = argv[INPUT_FILE_NAME_POS];
    output_file_name_ = argv[OUTPUT_FILE_NAME_POS];
    FilterDescriptor specific_filter;
    FilterDescriptor* a = new FilterDescriptor;
    for (size_t i = OUTPUT_FILE_NAME_POS + 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            if (i != OUTPUT_FILE_NAME_POS + 1) {
                storing_filters_and_their_params_.push_back(a);
            }
            a = new FilterDescriptor;
            (*a).filter_name = std::string_view(argv[i]).substr(1, std::string_view(argv[i]).size());
        } else {
            (*a).filter_params.push_back(std::string_view(argv[i]));
        }
    }
    storing_filters_and_their_params_.push_back(a);
}

std::string_view Parser::GetInputFileName() const {
    return input_file_name_;
}

std::string_view Parser::GetOutputFileName() const {
    return output_file_name_;
}
