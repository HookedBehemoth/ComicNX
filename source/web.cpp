#include "web.hpp"

namespace web {
    model::FType getFtype(const Value& imageJson) {
        std::string fTypeValue = imageJson["t"].GetString();
        if(fTypeValue == std::string("j")){
            return model::JPG;
        } else if(fTypeValue == std::string("p")){
            return model::PNG;
        } else if(fTypeValue == std::string("g")){
            return model::GIF;
        } else {
            printf("ERROR: %s is unsupported\n", fTypeValue.c_str());
            return model::UNSUPPORTED;
        }
    }
    model::tag getTag(const Value& tagJson) {
        model::tag tag = *new model::tag;
        tag.id = tagJson["id"].GetInt();
        tag.name = tagJson["name"].GetString();
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
            for(const Value& tag : tags.GetArray())
                comic->tags.push_back(getTag(tag));
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
    std::vector<model::comic> getComics(const Value& resultJson){
        std::vector<model::comic> comics;
        if(resultJson.IsArray())
            for(const Value& comic : resultJson.GetArray())
                comics.push_back(getComic(comic));
        
        return comics;
    }
    bool downloadFile(std::string url, std::string fileName){
        if(fs::fileExists(fileName)){
            return false;
        }
        printf("INFO: downloading %s to %s...\n", url.c_str(), fileName.c_str());
        swurl::WebRequest * request = new swurl::WebRequest(url.c_str());
        printf("INFO: finished making response");
        swurl::SessionManager::makeRequest(request);
        return fs::writeFile(fileName, request->response.rawResponseBody);
    }
    std::string getPath(model::comic comic, int page, bool thumb){
        std::string path = "comics/" + comic.id + "/" + std::to_string(page) + fs::getSuffix(comic.mediaFType[page+1]);
        if(fs::fileExists(path))
            return path;
        std::string url;
        if(thumb){
            url = FORMAT_THUMB + comic.mediaId + "/" + std::to_string(page) + "t" + fs::getSuffix(comic.mediaFType[page+1]);
        } else {
            url = FORMAT_IMG + comic.mediaId + "/" + std::to_string(page) + fs::getSuffix(comic.mediaFType[page+1]);
        }
        downloadFile(url, path);
        return path;
    }
    void onProgressChanged(swurl::WebRequest * request, double progress) {
        printf("INFO: progress=%g\n", progress);
    }
    void onCompleted(swurl::WebRequest * request) {
        printf("INFO: Download Completed with status code %d\n", request->response.statusCode);
    }
    void onError(swurl::WebRequest * request, std::string error) {
        printf("ERROR:  %s\n", error.c_str());
    }
}