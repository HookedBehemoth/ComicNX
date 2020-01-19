#include "nh/comic.hpp"
#include "nh/lang.hpp"
#include "utl.hpp"
#include "web.hpp"

#include <chrono>

namespace nh {

Comic::Comic(const nlohmann::json& json) {
    printf("creating comic...\n");
    this->id = getInt(json["id"]);
    this->mediaId = getInt(json["media_id"]);
    this->name = getString(json["title"]["pretty"]);
    this->cover = Image(json["images"]["cover"]);
    this->thumb = Image(json["images"]["thumbnail"]);
    this->timestamp = getInt(json["upload_date"]);
    this->pages = getInt(json["num_pages"]);
    this->favorites = getInt(json["num_favorites"]);

    const auto& imageValues = json["images"]["pages"];
    if(imageValues.is_array()) {
        for(const auto& imgVal : imageValues.array())
            this->images.push_back(Image(imgVal));
    }

    const auto& tagValues = json["tags"];
    if(tagValues.is_array()) {
        for (const auto& tagVal : tagValues.array()) {
            const auto tag = Tag(tagVal);
            this->tags.push_back(tag);
            if (tag.getType() == "language")
                this->language = Lang::FromString(tag.getName());
        }
    }
}

MemImage Comic::loadCover() const {
    return web::getFile(FORMAT_THUMB + std::to_string(this->mediaId) + "/cover" + this->cover.getExtension());
}

MemImage Comic::loadPreview() const {
    return web::getFile(FORMAT_THUMB + std::to_string(this->mediaId) + "/thumb" + this->cover.getExtension());
}

MemImage Comic::loadImage(int page, bool thumb) const {
    std::string url = thumb ?
        FORMAT_THUMB + std::to_string(this->mediaId) + "/" + std::to_string(page + 1) + "t" + this->images[page].getExtension():
        FORMAT_IMG + std::to_string(this->mediaId) + "/" + std::to_string(page + 1) + this->images[page].getExtension();
    return web::getFile(url);
}

std::vector<Tag> Comic::getTags() const {
    return this->tags;
}

Language Comic::getLanguage() const {
    return this->language;
}

std::string Comic::toString() const {
    return this->name;
}

static const long minute = 60;
static const long hour = minute * 60L;
static const long day = hour * 24L;
static const long month = day * 30;
static const long year = day * 365;

using namespace std::chrono;
std::string Comic::getRelativeTime() const {
    long currentTimeInSecs = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();

    std::string relativeString;
    long amount;
    long relative = currentTimeInSecs-timestamp;
    if (relative > year) {
        amount = relative/year;
        relativeString = "year";
    } else if (relative > month) {
        amount = relative/month;
        relativeString = "month";
    } else if (relative > day) {
        amount = relative/day;
        relativeString = "day";
    } else if (relative > hour) {
        amount = relative/hour;
        relativeString = "hour";
    } else if (relative > minute) {
        amount = relative/minute;
        relativeString = "minute";
    } else {
        amount = relative;
        relativeString = "second";
    }
    if (amount > 1){
        relativeString+="s";
    }
    return "Uploaded " + std::to_string(amount) + " " + relativeString + " ago";
}

}
