#include "web.hpp"

namespace web {
    model::CLang getLanguage(std::string langName) {
        if(langName == "japanese") return model::CLang::JP;
        if(langName == "chinese") return model::CLang::CN;
        if(langName == "english") return model::CLang::UK;
        return model::CLang::UNKNOWN;
    }
    model::FType getFtype(const Value& imageJson) {
        std::string fTypeValue = imageJson["t"].GetString();
        if(fTypeValue == std::string("j")){
            return model::JPG;
        } else if(fTypeValue == std::string("p")){
            return model::PNG;
        } else if(fTypeValue == std::string("g")){
            return model::GIF;
        } else {
            PRINTF("ERROR: %s is unsupported\n", fTypeValue.c_str());
            return model::FType::UNSUPPORTED;
        }
    }
    model::tag getTag(const Value& tagJson) {
        model::tag tag = *new model::tag;
        tag.name = tagJson["name"].GetString();
        tag.type = tagJson["type"].GetString();
        tag.id = tagJson["id"].GetInt();
        tag.count = tagJson["count"].GetInt();
        return tag;
    }
    model::comic getComic(const Value& comicJson) {
        model::comic * comic = new model::comic;
        const Value& idValue = comicJson["id"];
        switch(idValue.GetType()){
            case kStringType:
                comic->id = idValue.GetString();
                break;
            case kNumberType:
                comic->id = std::to_string(idValue.GetInt());
                break;
            default:
                break;
        }
        const Value& mediaIdValue = comicJson["media_id"];
        switch(mediaIdValue.GetType()){
            case kStringType:
                comic->mediaId = mediaIdValue.GetString();
                break;
            case kNumberType:
                comic->mediaId = std::to_string(mediaIdValue.GetInt());
                break;
            default:
                break;
        }
        comic->name = comicJson["title"]["pretty"].GetString();
        comic->mediaFType.push_back(getFtype(comicJson["images"]["cover"]));
        comic->mediaFType.push_back(getFtype(comicJson["images"]["thumbnail"]));
        comic->timestamp = comicJson["upload_date"].GetUint64();
        comic->pages = comicJson["num_pages"].GetInt();
        comic->favorites = comicJson["num_favorites"].GetInt();

        const Value& images = comicJson["images"]["pages"];
        if(images.IsArray()){
            for(const Value& image : images.GetArray())
                comic->mediaFType.push_back(getFtype(image));
        }
        const Value& tags = comicJson["tags"];
        if(tags.IsArray()){
            for(const Value& tagJson : tags.GetArray()){
                model::tag tag = getTag(tagJson);
                if((tag.type == "language") && ((tag.name == "japanese") || (tag.name == "chinese") || (tag.name == "english"))){
                    comic->language = getLanguage(tag.name);
                }
                comic->tags.push_back(tag);
            }
        }
        return *comic;
    }
    std::vector<model::comic> getComics(Document pageDocument){
        std::vector<model::comic> comics;
        if(pageDocument.HasMember("result")){
            const Value& result = pageDocument["result"];
            if(result.IsArray())
                for(const Value& comic : result.GetArray())
                    comics.push_back(getComic(comic));
        }
        return comics;
    }
    std::vector<model::comic> getComicsFromVal(const Value& resultJson){
        std::vector<model::comic> comics;
        if(resultJson.IsArray())
            for(const Value& comic : resultJson.GetArray())
                comics.push_back(getComic(comic));

        return comics;
    }
    model::page getComics(std::string url) {
        model::page *page = new model::page();
        PRINTF("INFO: searching with url: %s\n", url.c_str());
        swurl::WebRequest *request = new swurl::WebRequest(url);
        swurl::SessionManager::makeRequest(request);
        if(request->response.statusCode != 200) return *page;
        Document d;
        d.Parse(request->response.rawResponseBody.c_str());
        if(d.HasMember("error")) return *page;
        page->maxPages = d["num_pages"].GetInt();
        page->comics = getComicsFromVal(d["result"]);
        PRINTF("INFO: returning page with %lx comics\n", page->comics.size());
        return *page;
    }
    std::string getPath(model::comic comic, int page, bool thumb){
        std::string path = "sdmc:/switch/ComicNX/comics/" + comic.id + "/";
        fs::mkpath(path);
        path+=std::to_string(page);
        if(thumb) path+="t";
        path+=fs::getSuffix(comic.mediaFType[page+1]);
        if(fs::fileExists(path))
            return path;
        std::string url;
        if(thumb){
            url = FORMAT_THUMB + comic.mediaId + "/" + std::to_string(page) + "t" + fs::getSuffix(comic.mediaFType[page+1]);
        } else {
            url = FORMAT_IMG + comic.mediaId + "/" + std::to_string(page) + fs::getSuffix(comic.mediaFType[page+1]);
        }
        web::downloadFile(url, path);
        return path;
    }
    bool downloadFile(std::string url, std::string fileName){
        if(fs::fileExists(fileName)){
            return false;
        }
        PRINTF("INFO: downloading %s to %s...\n", url.c_str(), fileName.c_str());
        swurl::WebRequest * request = new swurl::WebRequest(url.c_str());
        PRINTF("INFO: finished making response");
        swurl::SessionManager::makeRequest(request);
        return fs::writeFile(fileName, request->response.rawResponseBody);
    }
    void onProgressChanged(swurl::WebRequest * request, double progress) {
        PRINTF("INFO: progress=%g\n", progress);
    }
    void onCompleted(swurl::WebRequest * request) {
        PRINTF("INFO: Download Completed with status code %lx\n", request->response.statusCode);
    }
    void onError(swurl::WebRequest * request, std::string error) {
        PRINTF("ERROR:  %s\n", error.c_str());
    }
}