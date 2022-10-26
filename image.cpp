#include "image.h"

Image::Image(int w, int h) {
    field_.resize(h, std::vector<Color>(w, {.R = 255, .G = 255, .B = 255}));
}

Image::Color& Image::GetPixel(int x, int y) {
    return field_[x][y];
}

Image::Color Image::GetPixel(int x, int y) const {
    return field_[x][y];
}

size_t Image::GetWidth() const {
    return field_.front().size();
}

size_t Image::GetHeight() const {
    return field_.size();
}

void Image::Resize(int w, int h) {
    field_.resize(h);
    for (auto i = 0; i < h; ++i) {
        field_[i].resize(w);
    }
}
