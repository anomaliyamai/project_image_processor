#pragma once
#include "image.h"
#include <cmath>

class BaseFilter {
public:
    virtual void Apply(Image& image) const = 0;
    virtual ~BaseFilter(){};
};

class CropFilter : public BaseFilter {
public:
    CropFilter(int height, int width) : new_height_(height), new_width_(width){};
    void Apply(Image& image) const override;
    ~CropFilter(){};

private:
    int new_height_ = 0;
    int new_width_ = 0;
};

class GreyScaleFilter : public BaseFilter {
public:
    void Apply(Image& image) const override;
    ~GreyScaleFilter(){};
};

class NegativeFilter : public BaseFilter {
public:
    void Apply(Image& image) const override;
    ~NegativeFilter(){};
};

class SharpeningFilter : public BaseFilter {
public:
    void Apply(Image& image) const override;
    ~SharpeningFilter(){};
};

class SobelOperatorFilter : public BaseFilter {
public:
    void Apply(Image& image) const override;
    ~SobelOperatorFilter(){};
};

class EdgeDetectionFilter : public GreyScaleFilter {
public:
    EdgeDetectionFilter(int threshold) : threshold_(threshold){};
    void Apply(Image& image) const override;
    ~EdgeDetectionFilter(){};

private:
    int threshold_ = 0;
};

class GaussianBlurFilter : public BaseFilter {
public:
    GaussianBlurFilter(int sigma) : new_sigma_(sigma) {
        FillMatrixCoefficients();
    };
    void Apply(Image& image) const override;
    ~GaussianBlurFilter(){};

private:
    void FillMatrixCoefficients();
    int new_sigma_ = 0;
    int matrix_size_ = 0;
    std::vector<double> matrix_gauss_;
    double coefficients_sum_ = 0;
};