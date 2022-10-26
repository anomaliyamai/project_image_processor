#pragma once
#include <cstdint>
#include <vector>

class Image {
public:
    struct Color {
        u_int8_t R = 0;
        u_int8_t G = 0;
        u_int8_t B = 0;
    };
    Image(int w, int h);
    void Resize(int w, int h);
    Color& GetPixel(int x, int y);
    Color GetPixel(int x, int y) const;
    size_t GetWidth() const;
    size_t GetHeight() const;

private:
    std::vector<std::vector<Color>> field_;
};
