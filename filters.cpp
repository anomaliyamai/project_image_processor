#include "filters.h"

void CropFilter::Apply(Image& image) const {
    if (image.GetHeight() < new_height_ || image.GetWidth() < new_width_) {
        throw std::invalid_argument("Wrong parameters for CropFilter, try again.");
    }
    image.Resize(new_height_, new_width_);
}

void GreyScaleFilter::Apply(Image& image) const {
    for (auto i = 0; i < image.GetHeight(); ++i) {
        for (auto j = 0; j < image.GetWidth(); ++j) {
            uint8_t changing_color =
                0.299 * image.GetPixel(i, j).R + 0.587 * image.GetPixel(i, j).G + 0.114 * image.GetPixel(i, j).B;
            image.GetPixel(i, j).R = changing_color;
            image.GetPixel(i, j).G = changing_color;
            image.GetPixel(i, j).B = changing_color;
        }
    }
}

void NegativeFilter::Apply(Image& image) const {
    for (auto i = 0; i < image.GetHeight(); ++i) {
        for (auto j = 0; j < image.GetWidth(); ++j) {
            image.GetPixel(i, j).R = 255 - image.GetPixel(i, j).R;
            image.GetPixel(i, j).G = 255 - image.GetPixel(i, j).G;
            image.GetPixel(i, j).B = 255 - image.GetPixel(i, j).B;
        }
    }
}

void SharpeningFilter::Apply(Image& image) const {
    Image resulting_picture = image;
    for (auto i = 0; i < image.GetHeight(); ++i) {
        for (auto j = 0; j < image.GetWidth(); ++j) {
            using CoordinateShift = std::pair<int, int>;
            std::vector<CoordinateShift> coordinates = {{0, -1}, {-1, 0}, {0, 0}, {1, 0}, {0, 1}};
            std::vector<int> coefficients = {-1, -1, 5, -1, -1};
            int new_red = 0;
            int new_green = 0;
            int new_blue = 0;
            size_t index = 0;
            for (const auto& pair : coordinates) {
                size_t x =
                    std::min(std::max(0, static_cast<int>(i) + pair.first), static_cast<int>(image.GetHeight()) - 1);
                size_t y =
                    std::min(std::max(0, static_cast<int>(j) + pair.second), static_cast<int>(image.GetWidth()) - 1);
                new_red += coefficients[index] * image.GetPixel(x, y).R;
                new_green += coefficients[index] * image.GetPixel(x, y).G;
                new_blue += coefficients[index] * image.GetPixel(x, y).B;
                index += 1;
            }
            resulting_picture.GetPixel(i, j).R = static_cast<uint8_t>(std::min(255, std::max(0, new_red)));
            resulting_picture.GetPixel(i, j).G = static_cast<uint8_t>(std::min(255, std::max(0, new_green)));
            resulting_picture.GetPixel(i, j).B = static_cast<uint8_t>(std::min(255, std::max(0, new_blue)));
        }
    }
    image = resulting_picture;
}

void EdgeDetectionFilter::Apply(Image& image) const {
    GreyScaleFilter::Apply(image);
    Image resulting_picture = image;
    for (auto i = 0; i < image.GetHeight(); ++i) {
        for (auto j = 0; j < image.GetWidth(); ++j) {
            using CoordinateShift = std::pair<int, int>;
            std::vector<CoordinateShift> coordinates = {{0, -1}, {-1, 0}, {0, 0}, {1, 0}, {0, 1}};
            std::vector<int> coefficients = {-1, -1, 4, -1, -1};
            int new_red = 0;
            int new_green = 0;
            int new_blue = 0;
            size_t index = 0;
            for (const auto& pair : coordinates) {
                size_t x =
                    std::min(std::max(0, static_cast<int>(i) + pair.first), static_cast<int>(image.GetHeight()) - 1);
                size_t y =
                    std::min(std::max(0, static_cast<int>(j) + pair.second), static_cast<int>(image.GetWidth()) - 1);
                new_red += coefficients[index] * image.GetPixel(x, y).R;
                new_green += coefficients[index] * image.GetPixel(x, y).G;
                new_blue += coefficients[index] * image.GetPixel(x, y).B;
                index += 1;
            }
            resulting_picture.GetPixel(i, j).R = static_cast<uint8_t>(std::min(255, std::max(0, new_red)));
            resulting_picture.GetPixel(i, j).G = static_cast<uint8_t>(std::min(255, std::max(0, new_green)));
            resulting_picture.GetPixel(i, j).B = static_cast<uint8_t>(std::min(255, std::max(0, new_blue)));
        }
    }
    image = resulting_picture;
    for (auto i = 0; i < image.GetHeight(); ++i) {
        for (auto j = 0; j < image.GetWidth(); ++j) {
            if (image.GetPixel(i, j).R > threshold_) {
                image.GetPixel(i, j).R = 255;
                image.GetPixel(i, j).G = 255;
                image.GetPixel(i, j).B = 255;
            } else {
                image.GetPixel(i, j).R = 0;
                image.GetPixel(i, j).G = 0;
                image.GetPixel(i, j).B = 0;
            }
        }
    }
}

void SobelOperatorFilter::Apply(Image& image) const {
    Image resulting_picture = image;
    for (auto i = 1; i < image.GetHeight() - 1; ++i) {
        for (auto j = 1; j < image.GetWidth() - 1; ++j) {
            resulting_picture.GetPixel(i, j).R =
                sqrt(std::pow(image.GetPixel(i - 1, j + 1).R - image.GetPixel(i - 1, j - 1).R +
                                  2 * (image.GetPixel(i, j + 1).R - image.GetPixel(i, j - 1).R) +
                                  image.GetPixel(i + 1, j + 1).R - image.GetPixel(i + 1, j - 1).R,
                              2) +
                     std::pow(image.GetPixel(i + 1, j - 1).R - image.GetPixel(i - 1, j - 1).R +
                                  2 * (image.GetPixel(i + 1, j).R - image.GetPixel(i - 1, j).R) +
                                  image.GetPixel(i + 1, j + 1).R - image.GetPixel(i - 1, j + 1).R,
                              2));
            resulting_picture.GetPixel(i, j).G =
                sqrt(std::pow(image.GetPixel(i - 1, j + 1).G - image.GetPixel(i - 1, j - 1).G +
                                  2 * (image.GetPixel(i, j + 1).G - image.GetPixel(i, j - 1).G) +
                                  image.GetPixel(i + 1, j + 1).G - image.GetPixel(i + 1, j - 1).G,
                              2) +
                     std::pow(image.GetPixel(i + 1, j - 1).G - image.GetPixel(i - 1, j - 1).G +
                                  2 * (image.GetPixel(i + 1, j).G - image.GetPixel(i - 1, j).G) +
                                  image.GetPixel(i + 1, j + 1).G - image.GetPixel(i - 1, j + 1).G,
                              2));
            resulting_picture.GetPixel(i, j).B =
                sqrt(std::pow(image.GetPixel(i - 1, j + 1).B - image.GetPixel(i - 1, j - 1).B +
                                  2 * (image.GetPixel(i, j + 1).B - image.GetPixel(i, j - 1).B) +
                                  image.GetPixel(i + 1, j + 1).B - image.GetPixel(i + 1, j - 1).B,
                              2) +
                     std::pow(image.GetPixel(i + 1, j - 1).B - image.GetPixel(i - 1, j - 1).B +
                                  2 * (image.GetPixel(i + 1, j).B - image.GetPixel(i - 1, j).B) +
                                  image.GetPixel(i + 1, j + 1).B - image.GetPixel(i - 1, j + 1).B,
                              2));
        }
    }
    image = resulting_picture;
}

void GaussianBlurFilter::Apply(Image& image) const {
    auto matrix_middle_pos = matrix_size_ / 2;
    Image resulting_picture = image;
    for (auto i = 0; i < resulting_picture.GetHeight(); ++i) {
        for (auto j = 0; j < resulting_picture.GetWidth(); ++j) {
            double r_sum = 0;
            double g_sum = 0;
            double b_sum = 0;
            for (auto k = 0; k < matrix_gauss_.size(); ++k) {
                int x = std::min(static_cast<int>(resulting_picture.GetHeight()) - 1,
                                 std::max(static_cast<int>(i + k) - static_cast<int>(matrix_middle_pos), 0));
                r_sum += static_cast<double>(resulting_picture.GetPixel(x, j).R) / 255 * matrix_gauss_[k];
                g_sum += static_cast<double>(resulting_picture.GetPixel(x, j).G) / 255 * matrix_gauss_[k];
                b_sum += static_cast<double>(resulting_picture.GetPixel(x, j).B) / 255 * matrix_gauss_[k];
            }
            r_sum /= coefficients_sum_;
            g_sum /= coefficients_sum_;
            b_sum /= coefficients_sum_;
            image.GetPixel(i, j).R = static_cast<uint8_t>(r_sum * 255.0);
            image.GetPixel(i, j).G = static_cast<uint8_t>(g_sum * 255.0);
            image.GetPixel(i, j).B = static_cast<uint8_t>(b_sum * 255.0);
        }
    }
    resulting_picture = image;
    for (auto i = 0; i < resulting_picture.GetHeight(); ++i) {
        for (auto j = 0; j < resulting_picture.GetWidth(); ++j) {
            double r_sum = 0;
            double g_sum = 0;
            double b_sum = 0;
            for (auto k = 0; k < matrix_gauss_.size(); ++k) {
                int y = std::min(static_cast<int>(resulting_picture.GetWidth()) - 1,
                                 std::max(static_cast<int>(j + k) - static_cast<int>(matrix_middle_pos), 0));
                r_sum += static_cast<double>(resulting_picture.GetPixel(i, y).R) / 255 * matrix_gauss_[k];
                g_sum += static_cast<double>(resulting_picture.GetPixel(i, y).G) / 255 * matrix_gauss_[k];
                b_sum += static_cast<double>(resulting_picture.GetPixel(i, y).B) / 255 * matrix_gauss_[k];
            }
            r_sum /= coefficients_sum_;
            g_sum /= coefficients_sum_;
            b_sum /= coefficients_sum_;
            image.GetPixel(i, j).R = static_cast<uint8_t>(r_sum * 255.0);
            image.GetPixel(i, j).G = static_cast<uint8_t>(g_sum * 255.0);
            image.GetPixel(i, j).B = static_cast<uint8_t>(b_sum * 255.0);
        }
    }
}

void GaussianBlurFilter::FillMatrixCoefficients() {
    matrix_size_ = 3 * 2 * new_sigma_ + 1;
    matrix_size_ = (matrix_size_ % 2 == 0) ? matrix_size_ + 1 : matrix_size_;
    matrix_gauss_.resize(matrix_size_);
    auto matrix_middle_pos = matrix_size_ / 2;
    for (auto i = 0; i < matrix_size_; ++i) {
        matrix_gauss_[i] = std::pow(M_E, static_cast<double>(-(i - matrix_middle_pos) * (i - matrix_middle_pos)) /
                                             (2 * new_sigma_ * new_sigma_)) /
                           std::sqrt(2 * M_PI * new_sigma_ * new_sigma_);
        coefficients_sum_ += matrix_gauss_[i];
    }
}
