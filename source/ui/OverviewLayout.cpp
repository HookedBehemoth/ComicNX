#include "OverviewLayout.hpp"
#include "MainApplication.hpp"

extern model::theme theme;
namespace ui {
    extern MainApplication *mainApp;
    model::comic comic;

    OverviewLayout::OverviewLayout() : pu::ui::Layout() {
        this->logo = new Image(4, 20, "romfs:/logo.png");
        logo->SetWidth(92);
        logo->SetHeight(60);
        this->SetBackgroundColor(theme.background);
        topBarRect = new Rectangle(0, 0, 1280, 100, theme.hoverColor);

        this->comicMenu = new Menu(20, 100, 1240, theme.hoverColor, 155, 4);
        this->Add(comicMenu);
        this->Add(topBarRect);    
        this->Add(logo);    
    }
    OverviewLayout::~OverviewLayout(){
        delete this->comicMenu;
        delete this->topBarRect;
        delete this->logo;
    }
    void OverviewLayout::catShowAll(int page){
        loadFromLink(web::FORMAT_ALL + std::to_string(page));
    }
    void OverviewLayout::catShowTagged(int tag, int page){
        loadFromLink(web::FORMAT_TAGGED + std::to_string(tag) + "&page=" + std::to_string(page));
    }
    void OverviewLayout::catShowSearch(std::string search, int page){
        loadFromLink(web::FORMAT_SEARCH + search + "&page=" + std::to_string(page));
    }
    void OverviewLayout::loadFromLink(std::string url){
        printf("searching with url: %s\n", url.c_str());
        swurl::WebRequest *request = new swurl::WebRequest(url);
        bool trying = true;
        while(trying){
            swurl::SessionManager::makeRequest(request);
            if(request->response.statusCode != 200){
                printf("error loading json from api...\n");
                int opts = mainApp->CreateShowDialog("Error", "couldn't fetch from web...", {"leave", "try again"}, false);
                if(opts == 1){
                    mainApp->Close();
                    return;
                }
            } else {
                trying = false;
            }
        }
        Document d;
        d.Parse(request->response.rawResponseBody.c_str());
        
        this->comics = web::getComics(d["result"]);
        loadComics();
    }
    void OverviewLayout::loadComics() {
        this->comicMenu->ClearItems();
        for(model::comic comic : this->comics) {
            MenuItem *item = new MenuItem(comic.name);
            item->SetColor(theme.textColor);
            std::string thumbPath = "comics/" + comic.id + "/thumb" + fs::getSuffix(comic.mediaFType[1]);  
            if(!fs::fileExists(thumbPath)){
                fs::mkpath("comics/" + comic.id + "/");
                web::downloadFile(web::FORMAT_THUMB + comic.mediaId + "/thumb"  + fs::getSuffix(comic.mediaFType[1]), thumbPath);
            }
            item->SetIcon(thumbPath);
            item->AddOnClick(std::bind(&OverviewLayout::onClick, this));
            this->comicMenu->AddItem(item);
        }
    }
    void OverviewLayout::onClick() {
        comic = comics[this->comicMenu->GetSelectedIndex()];
        mainApp->LoadLayout(mainApp->detailLayout);
        mainApp->detailLayout->showComicDetail();
    }
    void OverviewLayout::next() {
        mainApp->CreateShowDialog("stub", "stub!", {"ok"}, true);
    }
    void OverviewLayout::prev() {
        mainApp->CreateShowDialog("stub", "stub!", {"ok"}, true);
    }
}