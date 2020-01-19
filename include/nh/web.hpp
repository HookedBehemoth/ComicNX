#pragma once

#include "nh/comic.hpp"
#include "nh/theme.hpp"

namespace web {

std::vector<nh::Comic> getComics(const std::string &url, int &count);
MemImage getFile(const std::string &url);

}
