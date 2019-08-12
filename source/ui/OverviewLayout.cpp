#include "OverviewLayout.hpp"
#include "MainApplication.hpp"

extern model::theme theme;
namespace ui {
    extern MainApplication *mainApp;
    model::comic comic;

    OverviewLayout::OverviewLayout() : pu::ui::Layout() {
        this->logo = new Image(4, 20, "romfs:/logo.png");
        this->logo->SetWidth(92);
        this->logo->SetHeight(60);
        this->SetBackgroundColor(theme.background);
        this->topBarRect = new Rectangle(0, 0, 1280, 100, theme.hoverColor);
        this->comicMenu = new Menu(40, 120, 1200, theme.hoverColor, 150, 4);
        this->pageInfo = new TextBlock(600, 30, "loading...", 40);
        this->pageInfo->SetColor(theme.textColor);
        this->Add(comicMenu);
        this->Add(topBarRect);    
        this->Add(logo);
        this->Add(pageInfo);
    }
    OverviewLayout::~OverviewLayout(){
        delete this->comicMenu;
        delete this->topBarRect;
        delete this->logo;
    }
    void OverviewLayout::catShowAll(int page){
        this->mode = 0;
        this->page = page;
        loadFromLink(web::FORMAT_ALL + std::to_string(page));
    }
    void OverviewLayout::catShowSearch(std::string search, int page){
        this->searchString = search;
        this->mode = 1;
        this->page = page;
        loadFromLink(web::FORMAT_SEARCH + search + "&page=" + std::to_string(page));
    }
    void OverviewLayout::catShowTagged(model::tag tag, int page){
        this->currentTag = tag;
        this->mode = 2;
        this->page = page;
        loadFromLink(web::FORMAT_TAGGED + std::to_string(tag.id) + "&page=" + std::to_string(page));
    }
    void OverviewLayout::loadFromLink(std::string url){
        printf("searching with url: %s\n", url.c_str());
        swurl::WebRequest *request = new swurl::WebRequest(url);
        bool trying = true;
        while(trying){
            swurl::SessionManager::makeRequest(request);
            if(request->response.statusCode != 200){
                printf("error loading json from api...\n");
                int opts = mainApp->CreateShowDialog("Error", "couldn't fetch from web...", {"try again", "leave"}, false);
                if(opts == 1) {
                    mainApp->Close();
                    return;
                }
            } else {
                trying = false;
            }
        }
        Document d;
        d.Parse(request->response.rawResponseBody.c_str());
        if(d.HasMember("error")){
            int opts = mainApp->CreateShowDialog("invalid search", "your search was invalid...", {"different search", "leave"}, false);
            if(opts == 1) {
                mainApp->Close();
                return;
            } else {
                showOpts();
                return;
            }
        }
        this->maxPage = d["num_pages"].GetInt();
        this->comics = web::getComics(d["result"]);
        loadComics();
        std::string topStr;
        switch(mode) {
            case 0: topStr = "All"; break;
            case 1: topStr = this->searchString; break;
            case 2: topStr = this->currentTag.name; break;
        }
        topStr+=" ("+std::to_string(page)+"/"+std::to_string(maxPage)+")";
        this->pageInfo->SetText(topStr);
        this->pageInfo->SetX(640-(this->pageInfo->GetWidth()/2));
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
        if(this->page < maxPage) {
            int opts = mainApp->CreateShowDialog("next page?", "do you want to continue on the next page?", {"Yes", "Cancel"}, true);
            if(opts == 0)
                showPage(this->page + 1);
        }
    }
    void OverviewLayout::prev() {
        if(this->page > 1){
            int opts = mainApp->CreateShowDialog("previous page?", "do you want to continue on the previous page?", {"Yes", "Cancel"}, true);
            if(opts == 0)
                showPage(this->page - 1);
        }
    }
    void OverviewLayout::showPage(int i) {
        switch(this->mode){
            case 0: catShowAll(i); break;
            case 1: catShowSearch(this->searchString, i); break;
            case 2: catShowTagged(currentTag, i); break;
        }
    }
    void OverviewLayout::showOpts() {
        int opts = mainApp->CreateShowDialog("Category", "what category do you want to see?", {"all", "search", "tags (TODO)"}, true);
        if(opts == 0) {
            catShowAll(1);
        } else if(opts == 1) {
            search();
        } 
    }
    void OverviewLayout::search() {
        Result rc = 0;
        SwkbdConfig swkbd;
        char tmpstring[256];
        if(R_FAILED(rc = swkbdCreate(&swkbd, 0))){
            swkbdClose(&swkbd);
            return;
        }

        swkbdConfigMakePresetDefault(&swkbd);

        if (R_FAILED(rc = swkbdShow(&swkbd, tmpstring, sizeof(tmpstring)))) {
            swkbdClose(&swkbd);
            return;
        }

        swkbdClose(&swkbd);

        printf("output: %s\n", tmpstring);

        for(int i = 0; i < sizeof(tmpstring); i++) {
            if(tmpstring[i] == ' ')
                tmpstring[i] = '+';
        }

        catShowSearch(tmpstring, 1);
    }
}