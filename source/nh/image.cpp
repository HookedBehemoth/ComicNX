#include "nh/image.hpp"
#include "utl.hpp"

namespace nh {

Image::Image() {
    
}

Image::Image(const nlohmann::json &json) {
    const auto& ext = getString(json["t"]);
    if (ext == "j")
        this->type = this->JPEG;
    else if (ext == "p")
        this->type = this->PNG;
    //this->w = getInt("w");
    //this->h = getInt("h");
}

std::string Image::getExtension() const {
    printf("getting ext for %d\n", this->type);
    switch (this->type) {
    case JPEG:
        return ".jpg";
    case PNG:
        return ".png";
    default:
        return "";
    };
}

std::pair<int,int> Image::getDimensions() const {
    return { this->w, this->h };
}

}
