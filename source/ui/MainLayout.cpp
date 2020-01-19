#include "MainLayout.hpp"
#include "MainApplication.hpp"
#include "utl.hpp"
#include "nh/theme.hpp"
#include "nh/web.hpp"

namespace ui {
    extern MainApplication *mainApp;
    nh::Tag searchTag;
    nh::Comic comic;

    MainLayout::MainLayout() : pu::ui::Layout() {
        this->SetBackgroundColor(theme::back);
        this->topBarRect = Rectangle::New(0, 0, 1280, 100, theme::hover);
        this->logo = Image::New(14, 26, "romfs:/logo.png");
        this->logo->SetWidth(111);
        this->logo->SetHeight(48);
        this->topText = TextBlock::New(600, 30, "loading...", 40);
        this->topText->SetColor(theme::text);
        this->topText->SetHorizontalAlign(HorizontalAlign::Center);
        this->comicMenu = RichMenu::New(40, 120, 1200, theme::hover, 120, 5);
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
            printf("INFO: loading page %d...\n", this->page);
            loadPage();
        } else {
            printf("INFO: already at the end");
            return;
        }
    }

    void MainLayout::prev() {
        if(this->page > 1) {
            printf("INFO: loading page %d...\n", this->page);
            this->page--;
            loadPage();
        } else {
            printf("INFO: already at the start");
            return;
        }
    }

    void MainLayout::displayPage() {
        printf("DEBUG: %d/%d\n", page, maxPage);
        comicMenu->ClearItems();
        for(const auto& comic: comics) {
            printf("INFO: adding comic %s, %d, %d, %d\n", comic.toString().c_str(), comic.id, comic.mediaId, comic.pages);
            this->comicMenu->AddItem(comic);
        }
        this->comicMenu->SetSelectedIndex(0);
        std::string topStr, tmpString;
        tmpString = searchString;
        switch(this->searchMode) {
        case ALL:
            topStr+="all";
            break;
        case SEARCH:
            for(long unsigned int i = 0; i < sizeof(tmpString); i++) {
                if(tmpString[i] == '+')
                    tmpString[i] = ' ';
            }
            topStr += tmpString;
            break;
        case TAG:
            topStr += searchTag.getName();
            break;
        }
        topStr+=" sort by: ";
        if (this->searchMode != ALL && popular) topStr += "popular";
        else topStr+="date";
        topStr+=" (page: ";
        topStr+=std::to_string(page);
        topStr+="/";
        topStr+=std::to_string(maxPage);
        topStr+=")";
        this->topText->SetText(topStr);
    }

    void MainLayout::loadPage() {
        std::string url = FORMAT_API;
        char seperator = '?';
        switch(this->searchMode) {
        case ALL:
            url+="all";
            break;
        case SEARCH:
            url+="search";
            url+=seperator;
            url+="query=";
            url+=searchString;
            seperator='&';
            break;
        case TAG:
            url+="tagged";
            url+=seperator;
            url+="tag_id=";
            url+=std::to_string(searchTag.getId());
            seperator = '&';
            break;
        }
        url+=seperator;
        url+="page=";
        url+=std::to_string(page);
        if(popular){
            url+=seperator;
            url+="sort=popular";
        }
        this->comics = web::getComics(url, this->maxPage);
        displayPage();
    }

    void MainLayout::showOptions() {
        int opts = mainApp->CreateShowDialog("Category", "what category do you want to see?", {"all", "search", "popular-toggle", "tags (TODO)"}, true);
        if(opts == -1 || opts == -2) return;
        if(opts == 0) {
            this->searchMode = ALL;
        } else if(opts == 1) {
            if(!search()) return;
            this->page = 1;
            this->searchMode = SEARCH;
        } else if(opts == 2) {
            if(this->searchMode == ALL){
                mainApp->CreateShowDialog("not possible for all :/", "sadly the nhentai api doesn't allow this...", {"OK"}, true);
                return;
            }
            popular = !popular;
            page = 1;
        }
        loadPage();
    }

    bool MainLayout::search() {
        if(!canSwkbd()) {
            printf("ERROR: Can't launch swkbd because applet mempool is depleted...\n");
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
        printf("INFO: swkbd-output: %s\n", tmpstring);
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
        this->searchMode = TAG;
        this->page = 1;
        loadPage();
    }
}