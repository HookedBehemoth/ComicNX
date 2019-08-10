#pragma once

#include <string>
#include <vector>
#include <pu/ui/ui_Color.hpp>

namespace model{
    enum FType {
        JPG = 0,
        PNG = 1,
        GIF = 2,
        UNSUPPORTED = 2
    };
    struct tag{
        std::string name;
        int id;
    };
    struct comic {
        std::string name, id, mediaId;
        std::vector<int> mediaFType;
        std::vector<tag> tags;
        uint64_t timestamp;
        int pages, favorites;
    };
    struct theme {
        pu::ui::Color textColor, hoverColor, background, tagBg;
    };
}