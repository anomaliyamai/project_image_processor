#pragma once
#include <cstdint>
#include <fstream>
#include "image.h"

namespace bmp {
#pragma pack(push, 1)
struct BMPHeader {
    uint16_t file_type = 0;
    uint32_t file_size = 0;
    uint16_t reserved1 = 0;
    uint16_t reserved2 = 0;
    uint32_t offset_data = 0;
};

struct DIBHeader {
    uint32_t header_size = 40;
    struct Size {
        int32_t width = 0;
        int32_t height = 0;
    };
    Size size = {};
    uint16_t color_planes = 1;
    uint16_t bits_per_pixel = 24;
    uint32_t compression_method = 0;
    uint32_t bitmap_data_size = 0;
    struct Resolution {
        int32_t horizontal = 11811;
        int32_t vertical = 11811;
    };
    Resolution resolution = {};
    struct Colors {
        uint32_t total = 0;
        uint32_t important = 0;
    };
    Colors colors = {};
};
#pragma pack(pop)

Image LoadBMP(const std::string_view& path);
void SaveBMP(const Image& image, const std::string_view& path);
}  // namespace bmp
