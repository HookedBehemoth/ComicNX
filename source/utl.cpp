#include "utl.hpp"

using namespace std::chrono;
namespace utl {
    std::string getRelativeTime(uint64_t timestamp) {
        long currentTimeInSecs = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
        long minute = 60L;
        long hour = 3600L;
        long day = 86400L;
        long month = 2592000L;
        long year = 31536000L;

        std::string relativeString;
        int amount;
        long relative = currentTimeInSecs-timestamp;
        if(relative>year) {
            amount = (int)(relative/year);
            relativeString = "year";
        } else if(relative>month) {
            amount = (int)(relative/month);
            relativeString = "month";
        } else if(relative>day) {
            amount = (int)(relative/day);
            relativeString = "day";
        } else if(relative>hour) {
            amount = (int)(relative/hour);
            relativeString = "hour";
        } else if(relative>minute) {
            amount = (int)(relative/minute);
            relativeString = "minute";
        } else {
            amount = (int)relative;
            relativeString = "second";
        }
        if(amount>1){
            relativeString+="s";
        }
        return "Uploaded " + std::to_string(amount) + " " + relativeString + " ago";
    }

    model::CLang getLanguage(std::string langName) {
        if(langName == "japanese") return model::CLang::JP;
        if(langName == "chinese") return model::CLang::CN;
        if(langName == "english") return model::CLang::UK;
        return model::CLang::UNKNOWN;
    }
}