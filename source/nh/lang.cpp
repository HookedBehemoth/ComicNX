#include "nh/lang.hpp"

static const std::string LanguageStrings[] = {"Japan", "China", "UK", "US"};

namespace nh::Lang {

Language FromString(const std::string &str) {
    if(str == "japanese") return JP;
    else if(str == "chinese") return CN;
    else if(str == "english") return UK;
    else return UNKNOWN;
}

std::string toString(Language language) {
    return LanguageStrings[language];
}

std::string getFlag(Language language) {
    return "romfs:/" + LanguageStrings[language % 4] + ".png";
}

};