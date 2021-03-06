#include "MainLayout.hpp"
#include "MainApplication.hpp"
#include "utl.hpp"

extern model::theme theme;
namespace ui {
    extern MainApplication *mainApp;
    model::tag searchTag;
    model::comic comic;

    MainLayout::MainLayout() : pu::ui::Layout() {
        this->SetBackgroundColor(theme.background);
        this->topBarRect = Rectangle::New(0, 0, 1280, 100, theme.hoverColor);
        this->logo = Image::New(14, 26, "romfs:/logo.png");
        this->logo->SetWidth(111);
        this->logo->SetHeight(48);
        this->topText = TextBlock::New(600, 30, "loading...", 40);
        this->topText->SetColor(theme.textColor);
        this->topText->SetHorizontalAlign(HorizontalAlign::Center);
        this->comicMenu = RichMenu::New(40, 120, 1200, theme.hoverColor, 120, 5);
        this->comicMenu->SetCallback(std::bind(&MainLayout::onItemClick, this));
        this->Add(this->topBarRect);    
        this->Add(this->logo);
        this->Add(this->topText);
        this->Add(this->comicMenu);
    }

    void MainLayout::onInput(u64 Down, u64 Up, u64 Held, pu::ui::Touch Pos) {
        if((Down & KEY_PLUS) || (Down & KEY_MINUS)){
            mainApp->Close();
        }
        if(Down & KEY_B) {
            int opt = mainApp->CreateShowDialog("Exit?", "Do you really want to quit?", {"yes", "no"}, true, "romfs:/shrek.png");
            if(opt == 0){
                mainApp->Close();
            }
        }
        if(Down & KEY_X) {
            this->showOptions();
        }
        if((Down & KEY_RIGHT) || (Down & KEY_ZR) || (Down & KEY_R)) {
            this->next();
        }
        if((Down & KEY_LEFT) || (Down & KEY_ZL) || (Down & KEY_L)) {
            this->prev();
        }
    }
    void MainLayout::next() {
        if (this->page < this->maxPage) {
            this->page++;
            PRINTF("INFO: loading page %d...\n", this->page);
            loadPage();
        } else {
            PRINTF("INFO: already at the end");
            return;
        }
    }
    void MainLayout::prev() {
        if(this->page > 1) {
            PRINTF("INFO: loading page %d...\n", this->page);
            this->page--;
            loadPage();
        } else {
            PRINTF("INFO: already at the start");
            return;
        }
    }
    void MainLayout::displayPage() {
        PRINTF("DEBUG: %d/%d\n", page, maxPage);
        comicMenu->ClearItems();
        for(model::comic comic: comics) {
            PRINTF("INFO: adding comic %s\n", comic.id.c_str());;
            this->comicMenu->AddItem(new model::comic(comic));
        }
        this->comicMenu->SetSelectedIndex(0);
        std::string topStr, tmpString;
        tmpString = searchString;
        switch(mode) {
            case model::searchMode::ALL:    topStr+="all";          break;
            case model::searchMode::SEARCH:{
                for(long unsigned int i = 0; i < sizeof(tmpString); i++) {
                    if(tmpString[i] == '+')
                        tmpString[i] = ' ';
                }
                topStr+=tmpString;
                break;
            }
            case model::searchMode::TAG:{
                topStr+=searchTag.name;
                break;
            }
        }
        topStr+=" sort by: ";
        if(mode!=model::searchMode::ALL && popular) topStr+="popular";
        else topStr+="date";
        topStr+=" (page: ";
        topStr+=std::to_string(page);
        topStr+="/";
        topStr+=std::to_string(maxPage);
        topStr+=")";
        this->topText->SetText(topStr);
    }
    void MainLayout::loadPage() {
        std::string url = web::FORMAT_API;
        char seperator = '?';
        switch(mode) {
            case model::searchMode::ALL: url+="all"; break;
            case model::searchMode::SEARCH:
            {
                url+="search";
                url+=seperator;
                url+="query=";
                url+=searchString;
                seperator='&';
                break;
            }
            case model::searchMode::TAG:{
                url+="tagged";
                url+=seperator;
                url+="tag_id=";
                url+=std::to_string(searchTag.id);
                seperator = '&';
                break;
            }
        }
        url+=seperator;
        url+="page=";
        url+=std::to_string(page);
        if(popular){
            url+=seperator;
            url+="sort=popular";
        }
        model::page page = web::getComics(url);

        this->maxPage = page.maxPages;
        this->comics = page.comics;
        displayPage();
    }
    void MainLayout::showOptions() {
        int opts = mainApp->CreateShowDialog("Category", "what category do you want to see?", {"all", "search", "popular-toggle", "tags (TODO)"}, true);
        if(opts == -1 || opts == -2) return;
        if(opts == 0) {
            mode = model::searchMode::ALL;
        } else if(opts == 1) {
            if(!search()) return;
            this->page = 1;
            this->mode = model::searchMode::SEARCH;
        } else if(opts == 2) {
            if(mode == 0){
                mainApp->CreateShowDialog("not possible for all :/", "sadly the nhentai api doesn't allow this...", {"OK"}, true);
                return;
            }
            popular = !popular;
            page = 1;
        }
        loadPage();
    }
    bool MainLayout::search() {
        if(!utl::canSwkbd()) {
            PRINTF("ERROR: Can't launch swkbd because applet mempool is depleted...\n");
            mainApp->CreateShowDialog("Not possible in this session","You can't access the softwarekeyboard because your CFW doesn't reserve enough memory for it.\n\nIf you use Atmosphère update to at least 0.9.3 or use titleoverride.\n\nIf you use any other CFW use titleoverride (hold down R while starting a game) or switch to Atmosphère.", {"OK"}, true);
            return false;
        }
        Result rc = 0;
        SwkbdConfig swkbd;
        char tmpstring[256];
        if(R_FAILED(rc = swkbdCreate(&swkbd, 0))){
            swkbdClose(&swkbd);
            return false;
        }
        swkbdConfigMakePresetDefault(&swkbd);
        if (R_FAILED(rc = swkbdShow(&swkbd, tmpstring, sizeof(tmpstring)))) {
            swkbdClose(&swkbd);
            return false;
        }
        swkbdClose(&swkbd);
        PRINTF("INFO: swkbd-output: %s\n", tmpstring);
        for(long unsigned int i = 0; i < sizeof(tmpstring); i++)
            if(tmpstring[i] == ' ')
                tmpstring[i] = '+';
        searchString = tmpstring;
        if(tmpstring[0] != '\0') return true;
        else return false;
    }
    void MainLayout::onItemClick() {
        comic = this->comics[this->comicMenu->GetSelectedIndex()];
        mainApp->LoadLayout(mainApp->detailLayout);
        mainApp->detailLayout->showComicDetail();
    }
    void MainLayout::tagSearch() {
        this->mode = model::searchMode::TAG;
        this->page = 1;
        loadPage();
    }
}