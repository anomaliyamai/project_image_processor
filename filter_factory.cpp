#include "filter_factory.h"
#include <stdexcept>
#include <string>

namespace filter_factories {
BaseFilter* MakeGaussianBlurFilter(const FilterDescriptor& fd) {
    if (fd.filter_params.size() != 1) {
        throw std::invalid_argument("Wrong Gaussian Blur filter parameters size.");
    }
    int sigma = std::stoi(static_cast<std::string>(fd.filter_params[0]));
    return new GaussianBlurFilter(sigma);
}

BaseFilter* MakeEdgeDetectionFilter(const FilterDescriptor& fd) {
    if (fd.filter_params.size() != 1) {
        throw std::invalid_argument("Wrong Edge Detection filter parameters size.");
    }
    int sigma = std::stoi(static_cast<std::string>(fd.filter_params[0]));
    return new EdgeDetectionFilter(sigma);
}

BaseFilter* MakeNegativeFilter(const FilterDescriptor& fd) {
    return new NegativeFilter;
}

BaseFilter* MakeSobelOperatorFilter(const FilterDescriptor& fd) {
    return new SobelOperatorFilter;
}

BaseFilter* MakeGrayscaleFilter(const FilterDescriptor& fd) {
    return new GreyScaleFilter;
}

BaseFilter* MakeSharpeningFilter(const FilterDescriptor& fd) {
    return new SharpeningFilter;
}

BaseFilter* MakeCropFilter(const FilterDescriptor& fd) {
    if (fd.filter_params.size() != 2) {
        throw std::invalid_argument("Wrong Crop filter params size.");
    }
    int width = std::stoi(static_cast<std::string>(fd.filter_params[0]));
    int height = std::stoi(static_cast<std::string>(fd.filter_params[1]));
    return new CropFilter(width, height);
}
}  // namespace filter_factories

FilterPipelineFactory::FilterFactory FilterPipelineFactory::GetFilterMaker(std::string_view name) const {
    FilterToMakerMap::const_iterator it = filter_to_makers_.find(name);
    if (it != filter_to_makers_.end()) {
        return it->second;
    }
    return nullptr;
}

BaseFilter* FilterPipelineFactory::CreateFilter(const FilterDescriptor& descriptor) const {
    FilterFactory gear = GetFilterMaker(descriptor.filter_name);
    if (!gear) {
        return nullptr;
    }
    BaseFilter* current_filter = gear(descriptor);
    return current_filter;
}
