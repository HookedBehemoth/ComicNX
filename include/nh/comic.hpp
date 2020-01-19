#pragma once

#include "nh/tag.hpp"
#include "nh/lang.hpp"
#include "nh/image.hpp"
#include "web.hpp"

#include <switch/types.h>
#include "json.hpp"
#include <string>
#include <vector>

namespace nh {

class Comic {
private:
    std::string name;
    std::vector<Tag> tags;
    Image cover, thumb;
    std::vector<Image> images;
    u64 timestamp;
    Language language;
public:
    int id, mediaId;
    u32 pages, favorites;
    Comic() {};
    Comic(const nlohmann::json& json);
    MemImage loadCover() const;
    MemImage loadPreview() const;
    MemImage loadImage(int position, bool thumb = false) const;
    std::vector<Tag> getTags() const;
    Language getLanguage() const;
    std::string toString() const;
    std::string getRelativeTime() const;
};

}
