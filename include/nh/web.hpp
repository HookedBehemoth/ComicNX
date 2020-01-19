#pragma once

#include "nh/comic.hpp"

struct MemImage {
    char *memory;
    size_t size;
};

namespace web {

static const std::string FORMAT_THUMB = "https://t.nhentai.net/galleries/";
static const std::string FORMAT_IMG = "https://i.nhentai.net/galleries/";
static const std::string FORMAT_API = "https://nhentai.net/api/galleries/";
std::vector<nh::Comic> getComics(const std::string &url, int &count);
MemImage getFile(const std::string &url);

}
