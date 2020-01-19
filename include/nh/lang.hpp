#pragma once

#include <string>

namespace nh {

typedef enum {
    JP,
    CN,
    UK,
    US,
    UNKNOWN
} Language;

namespace Lang {
    Language FromString(const std::string &str);
    std::string toString(Language language);
    std::string getFlag(Language language);
};

}
