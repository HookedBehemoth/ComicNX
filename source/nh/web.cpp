#include "web.hpp"
#include "utl.hpp"

#include "json.hpp"
#include <stdlib.h>
#include <curl/curl.h>

static size_t StringWrite(char *contents, size_t size, size_t nmemb, void *userdata) {
	((std::string*)userdata)->append(contents, size * nmemb);
    return size * nmemb;
}
 
static size_t MemWrite(char *contents, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  MemImage *mem = (MemImage *)userp;
 
  char *ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
  if (!ptr) {
    /* out of memory! */ 
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}

namespace web {

template<class T>
T download(const std::string &url, curl_write_callback cb) {
    T val;
    CURL *easy = curl_easy_init();
    if (easy) {
        curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, cb);
        curl_easy_setopt(easy, CURLOPT_WRITEDATA, &val);
        curl_easy_setopt(easy, CURLOPT_URL, url.c_str());
        curl_easy_setopt(easy, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(easy, CURLOPT_SSL_VERIFYPEER, 0L);
        CURLcode res = curl_easy_perform(easy);
        long http_code = 0;
        curl_easy_getinfo(easy, CURLINFO_RESPONSE_CODE, &http_code);
        curl_easy_cleanup(easy);
        if (res == CURLE_OK && http_code == 200)
            return val;
    }
    return T();
}

std::vector<nh::Comic> getComics(const std::string &url, int &count) {
    std::vector<nh::Comic> page;
    printf("INFO: searching with url: %s\n", url.c_str());
    const auto jstr = download<std::string>(url, StringWrite);
    const auto json = nlohmann::json::parse(jstr);
    if(json.contains("error")) return page;
    count = getInt(json["num_pages"]);
    const auto &comicVals = json["result"];
    if (comicVals.is_array()) {
        for (const auto &comicVal : comicVals)
            page.push_back(nh::Comic(comicVal));
    }
    printf("INFO: returning page with %lx comics\n", page.size());
    return page;
}

MemImage getFile(const std::string &url) {
    return download<MemImage>(url, MemWrite);
}

}
