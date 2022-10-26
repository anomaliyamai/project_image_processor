#pragma once

#include "filters.h"
#include <map>
#include "parser.h"

namespace filter_factories {
BaseFilter* MakeGaussianBlurFilter(const FilterDescriptor& fd);
BaseFilter* MakeNegativeFilter(const FilterDescriptor& fd);
BaseFilter* MakeGrayscaleFilter(const FilterDescriptor& fd);
BaseFilter* MakeSharpeningFilter(const FilterDescriptor& fd);
BaseFilter* MakeEdgeDetectionFilter(const FilterDescriptor& fd);
BaseFilter* MakeCropFilter(const FilterDescriptor& fd);
BaseFilter* MakeSobelOperatorFilter(const FilterDescriptor& fd);
}  // namespace filter_factories

class FilterPipelineFactory {
public:
    using FilterFactory = BaseFilter* (*)(const FilterDescriptor&);
    using FilterToMakerMap = std::map<std::string_view, FilterFactory>;
    FilterPipelineFactory()
        : filter_to_makers_({{"crop", &filter_factories::MakeCropFilter},
                             {"neg", &filter_factories::MakeNegativeFilter},
                             {"gs", &filter_factories::MakeGrayscaleFilter},
                             {"sharp", &filter_factories::MakeSharpeningFilter},
                             {"edge", &filter_factories::MakeEdgeDetectionFilter},
                             {"blur", &filter_factories::MakeGaussianBlurFilter},
                             {"sobel", &filter_factories::MakeSobelOperatorFilter}}) {
    }
    BaseFilter* CreateFilter(const FilterDescriptor& descriptor) const;

protected:
    FilterFactory GetFilterMaker(std::string_view name) const;
    FilterToMakerMap filter_to_makers_;
};

