#pragma once

#include <string>
#include <vector>
#include <pu/ui/ui_Color.hpp>

namespace model{
    enum FType {JPG, PNG, GIF, UNSUPPORTED};
    static const std::string FTypeStrings[] = {".jpg", ".png", ".gif", ".unsupported"};
    enum CLang {JP, CN, UK, US, UNKNOWN};
    static const std::string CLangStrings[] = {"Japan", "China", "UK", "US"};
    struct tag{
        std::string name;
        std::string type;
        int id;
        int count;
        inline bool operator==(tag a) {
            return (a.id == id);
        };
    };
    struct comic {
        std::string name, id, mediaId;
        std::vector<int> mediaFType;
        std::vector<tag> tags;
        uint64_t timestamp;
        int pages, favorites;
        int language = CLang::UNKNOWN;
    };
    struct page {
        std::vector<model::comic> comics;
        int maxPages;
    };
    struct theme {
        pu::ui::Color textColor, hoverColor, background, tagBg;
    };
    enum searchMode {
        ALL, SEARCH, TAG
    };
}