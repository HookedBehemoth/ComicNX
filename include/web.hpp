#pragma once

#include "comicnx.hpp"
#include <Swurl.hpp>
#include <vector>
#include <switch.h>
#include "fs.hpp"
#include "utl.hpp"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;
namespace web {
    const std::string FORMAT_SEARCH = "https://nhentai.net/api/galleries/search?query=";
    const std::string FORMAT_TAGGED = "https://nhentai.net/api/galleries/tagged?tag_id=";
    const std::string FORMAT_ALL = "https://nhentai.net/api/galleries/all?page=";
    const std::string FORMAT_THUMB = "https://t.nhentai.net/galleries/";
    const std::string FORMAT_IMG = "https://i.nhentai.net/galleries/";
    const std::string FORMAT_API = "https://nhentai.net/api/galleries/";
    model::comic getComic(Document pageDocument);
    model::comic getComic(const Value& comicJson);
    std::vector<model::comic> getComics(const Value& resultJson);
    std::vector<model::comic> getComics(Document pageDocument);
    model::page getComics(std::string url);
    std::string getPath(model::comic comic, int page, bool thumb);
    bool downloadFile(std::string url, std::string path);
    void onProgressChanged(swurl::WebRequest * request, double progress);
    void onCompleted(swurl::WebRequest * request);
    void onError(swurl::WebRequest * request, std::string error);
}