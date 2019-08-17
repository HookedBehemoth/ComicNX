#pragma once

#include "comicnx.hpp"
#include <Swurl.hpp>
#include "fs.hpp"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;
namespace web {
    static std::string FORMAT_THUMB = "https://t.nhentai.net/galleries/";
    static std::string FORMAT_IMG = "https://i.nhentai.net/galleries/";
    static std::string FORMAT_API = "https://nhentai.net/api/galleries/";
    model::page getComics(std::string url);
    std::string getPath(model::comic comic, int page, bool thumb);
    bool downloadFile(std::string url, std::string path);
    void onProgressChanged(swurl::WebRequest * request, double progress);
    void onCompleted(swurl::WebRequest * request);
    void onError(swurl::WebRequest * request, std::string error);
}