#include "nh/tag.hpp"

namespace nh {

Tag::Tag(const nlohmann::json &json) {
    this->name = json["name"].get<std::string>();
    this->type = json["type"].get<std::string>();
    this->id = json["id"].get<int>();
    this->count = json["count"].get<int>();
}

std::string Tag::getName() const {
    return this->name;
}

std::string Tag::getType() const {
    return this->type;
}

int Tag::getId() const {
    return this->id;
}

int Tag::getCount() const {
    return this->count;
}

std::string Tag::toString() const {
    return this->name + " (" + std::to_string(this->count) + ")";
}

}
