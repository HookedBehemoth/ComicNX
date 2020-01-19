#pragma once

#include "json.hpp"
#include <string>

namespace nh {

class Image {
private:
    enum {
        JPEG, PNG, UNKNOWN
    } type = UNKNOWN;
    int w, h;
public:
    Image() {};
    Image(const nlohmann::json &json);
    std::string getExtension() const;
    std::pair<int,int> getDimensions() const;
};

}
