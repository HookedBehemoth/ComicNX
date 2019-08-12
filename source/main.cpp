#include <string>
#include <switch.h>
#include <pu/Plutonium>
#include <stdio.h>
#include <Swurl.hpp>
#include "MainApplication.hpp"
#include "web.hpp"
#include <thread>

#define DEBUG 1
using namespace swurl;
model::theme theme;
static bool comic;
static model::comic newComic;

static void dlComics(void* args) {
    WebRequest * request = new WebRequest(web::FORMAT_ALL + "1");
    SessionManager::makeRequest(request);
    Document d;
    d.Parse(request->response.rawResponseBody.c_str());
    std::vector<model::comic> comics = web::getComics(d["result"]);
    /*for(model::comic comic : comics) {
        web::getPath(comic, 1, false);
        *newComic = comic;
    } */
    fs::mkpath("comics/" + comics[0].id + "/");
    web::getPath(comics[0], 1, false);
    newComic = comics[0];
    comic = true;
    
    /*const Value& comicsJson = d["result"];
    if(comicsJson.IsArray());
    for(const Value& comicJson : comicsJson.GetArray()) {
        model::comic comic = web::getComic(comicJson);
        web::getPath(comic, 1, false);
        *newComic = comic;
        //comics.push_back(comic);
        //newComic = true;
    }*/
}
void init(){
    printf("setting theme");
    theme = *new model::theme;
    theme.background = pu::ui::Color::FromHex("#0d0d0dff");
    theme.hoverColor = pu::ui::Color::FromHex("#1f1f1fff");
    theme.tagBg = pu::ui::Color::FromHex("#666666ff");
    theme.textColor = pu::ui::Color::FromHex("#d9d9d9ff");
}
int main(int argc, char **argv) {
    socketInitializeDefault();
    SessionManager::initialize();
    SessionManager::userAgent = "gui-test/0.9.0";
    SessionManager::requestHeaders.insert(
        std::pair<std::string, std::string>(
            "Cache-Control",
            "no-cache"
        )
    );
    SessionManager::onProgressChanged = web::onProgressChanged;
    SessionManager::onCompleted = web::onCompleted;
    SessionManager::onError = web::onError;
    #ifdef DEBUG
        nxlinkStdio();
    #endif

    /*init();

    printf("starting application...\n");
    ui::MainApplication *amain = new ui::MainApplication();
    amain->Show();
    printf("cleaning up...\n");
    delete amain; */

    //newComic = nullptr;
    comic = false;
    printf("creating thread...\n");
    Thread dlThread;
    threadCreate(&dlThread, dlComics, nullptr, 0x2000, 0x2C, -2);
    threadStart(&dlThread);

    while(appletMainLoop()) {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS)
            break;
        
        if(comic) {
            printf("\n\nnew Comic\n\n");
            //for(model::comic comic : comics) {
                printf("comic: %s, %s, %s\n",
                        newComic.name.c_str(),
                        newComic.id.c_str(),
                        newComic.mediaId.c_str());
            //}
            //newComic = nullptr;
        }

    }
    threadWaitForExit(&dlThread);
    threadClose(&dlThread);
    SessionManager::dealloc();
    printf("exiting...\n");
    return 0;
}