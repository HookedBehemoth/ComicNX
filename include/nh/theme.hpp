#pragma once

#include <pu/ui/ui_Types.hpp>

class MemImage {
public:
    MemImage() {
        this->memory = (char*)malloc(1);
        this->size = 0;
    }
    /*~MemImage() {
        free(memory);
    }*/
    char *memory;
    size_t size;
};
static const std::string FORMAT_THUMB = "https://t.nhentai.net/galleries/";
static const std::string FORMAT_IMG = "https://i.nhentai.net/galleries/";
static const std::string FORMAT_API = "https://nhentai.net/api/galleries/";

namespace theme {

static const pu::ui::Color back = { 0x0d, 0x0d, 0x0d, 0xff };
static const pu::ui::Color hover = { 0x1f, 0x1f, 0x1f, 0xff };
static const pu::ui::Color tag = { 0x66, 0x66, 0x66, 0xff };
static const pu::ui::Color text = { 0xd9, 0xd9, 0xd9, 0xff };

}
