#include "bmp.h"
#include "filters.h"
#include "filter_factory.h"
#include "parser.h"

int main(int argc, char* argv[]) {
    auto image = bmp::LoadBMP("/Users/olegafanasev/cpp-base-hse-2022/projects/image_processor/examples/example.bmp");
    BaseFilter* mm;
    SobelOperatorFilter g;
    mm = &g;
    mm->Apply(image);
    bmp::SaveBMP(image, "/Users/olegafanasev/cpp-base-hse-2022/projects/image_processor/examples/applied_custom_filter.bmp");
}
