#include "bmp.h"
#include "image.h"
#include "reading_functions.h"

bmp::BMPHeader ReadBMPHeader(std::ifstream& in) {
    bmp::BMPHeader header;
    ReadNum(in, header.file_type);
    ReadNum(in, header.file_size);
    ReadNum(in, header.reserved1);
    ReadNum(in, header.reserved2);
    ReadNum(in, header.offset_data);
    return header;
}

bmp::DIBHeader ReadDIBHeader(std::ifstream& in) {
    bmp::DIBHeader dibheader;
    ReadNum(in, dibheader.header_size);
    ReadNum(in, dibheader.size.width);
    ReadNum(in, dibheader.size.height);
    ReadNum(in, dibheader.color_planes);
    ReadNum(in, dibheader.bits_per_pixel);
    ReadNum(in, dibheader.compression_method);
    ReadNum(in, dibheader.bitmap_data_size);
    ReadNum(in, dibheader.resolution.horizontal);
    ReadNum(in, dibheader.resolution.vertical);
    ReadNum(in, dibheader.colors.important);
    ReadNum(in, dibheader.colors.total);
    return dibheader;
}

void WriteBMPHeader(std::ofstream& out, const bmp::BMPHeader& header) {
    WriteNum(out, header.file_type);
    WriteNum(out, header.file_size);
    WriteNum(out, header.reserved1);
    WriteNum(out, header.reserved2);
    WriteNum(out, header.offset_data);
}

void WriteDIBHeader(std::ofstream& out, const bmp::DIBHeader& header) {
    WriteNum(out, header.header_size);
    WriteNum(out, header.size.width);
    WriteNum(out, header.size.height);
    WriteNum(out, header.color_planes);
    WriteNum(out, header.bits_per_pixel);
    WriteNum(out, header.compression_method);
    WriteNum(out, header.bitmap_data_size);
    WriteNum(out, header.resolution.horizontal);
    WriteNum(out, header.resolution.vertical);
    WriteNum(out, header.colors.important);
    WriteNum(out, header.colors.total);
}

Image bmp::LoadBMP(const std::string_view& path) {
    std::ifstream in{path, std::ios::binary};
    if (!in.is_open()) {
        throw std::invalid_argument("Failed to open file, try again.");
    }
    BMPHeader header = ReadBMPHeader(in);
    uint16_t checking_if_file_type_is_bmp = 0x4d42;
    if (header.file_type != checking_if_file_type_is_bmp) {
        throw std::invalid_argument("Unsuitable open file type, try again.");
    }
    DIBHeader dib_header = ReadDIBHeader(in);
    if (dib_header.header_size != 40) {
        throw std::invalid_argument("Incorrect size of header.");
    }
    Image image{dib_header.size.width, dib_header.size.height};
    const size_t padding = (4 - (dib_header.size.width * dib_header.bits_per_pixel) % 4) % 4;
    for (auto i = 0; i < dib_header.size.height; ++i) {
        for (auto j = 0; j < dib_header.size.width; ++j) {
            Image::Color el;
            ReadNum(in, el.R);
            ReadNum(in, el.G);
            ReadNum(in, el.B);
            image.GetPixel(dib_header.size.height - i - 1, j) = el;
        }
        in.ignore(padding);
    }
    return image;
}

void bmp::SaveBMP(const Image& image, const std::string_view& path) {
    BMPHeader bmpheader;
    bmpheader.file_type = 0x4d42;
    bmpheader.file_size = sizeof(BMPHeader) + sizeof(DIBHeader) + image.GetHeight() * image.GetWidth() * 3;
    bmpheader.offset_data = sizeof(BMPHeader) + sizeof(DIBHeader);

    DIBHeader dibheader;
    dibheader.size.width = image.GetWidth();
    dibheader.size.height = image.GetHeight();
    auto row_length = image.GetWidth() * 3;
    auto padding = ((row_length + 3) / 4) * 4 - row_length;
    dibheader.bitmap_data_size = image.GetHeight() * (row_length + padding);

    std::ofstream out{path, std::ofstream::binary};
    WriteBMPHeader(out, bmpheader);
    WriteDIBHeader(out, dibheader);
    for (int i = static_cast<size_t>(image.GetHeight() - 1); i >= 0; --i) {
        for (auto j = 0; j < image.GetWidth(); ++j) {
            WriteNum(out, image.GetPixel(i, j).R);
            WriteNum(out, image.GetPixel(i, j).G);
            WriteNum(out, image.GetPixel(i, j).B);
        }
        for (auto j = 0; j < padding; ++j) {
            WriteNum<char>(out, 0);
        }
    }
    out.close();
}

