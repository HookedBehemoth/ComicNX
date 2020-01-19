#pragma once

#include "json.hpp"
#include <string>

namespace nh {

class Tag {
private:
    std::string name, type;
    int id, count;
public:
    Tag() {};
    Tag(const nlohmann::json &json);
    std::string getName() const;
    std::string getType() const;
    int getId() const;
    int getCount() const;
    std::string toString() const;
    inline bool operator==(const Tag &a) const {
        return a.type == this->type && a.id == this->id;
    }
};

}
