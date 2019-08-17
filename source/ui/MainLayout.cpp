#include "MainLayout.hpp"
#include "MainApplication.hpp"

extern model::theme theme;
namespace ui {
    extern MainApplication *mainApp;
    model::tag searchTag;
    model::comic comic;

    MainLayout::MainLayout() : pu::ui::Layout() {
        this->SetBackgroundColor(theme.background);
        this->topBarRect = new Rectangle(0, 0, 1280, 100, theme.hoverColor);
        this->logo = new Image(14, 26, "romfs:/logo.png");
        this->logo->SetWidth(111);
        this->logo->SetHeight(48);
        this->topText = new TextBlock(600, 30, "loading...", 40);
        this->topText->SetColor(theme.textColor);
        this->comicMenu = new Menu(40, 120, 1200, theme.hoverColor, 120, 5);
        this->comicMenu->AddItem(new MenuItem("fuck"));
        this->Add(topBarRect);    
        this->Add(logo);
        this->Add(topText);
        this->Add(comicMenu);
    }
    MainLayout::~MainLayout() {
        delete this->comicMenu;
        delete this->topBarRect;
        delete this->logo;
        delete this->topText;
    }
    void MainLayout::onInput(u64 Down, u64 Up, u64 Held) {
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
        PRINTF("INFO: switched from page %d and section %d to...\n", this->page, this->section);
        if(this->section == (this->maxSection-1)) {
            if (this->page < this->maxPage) {
                this->page++;
                this->section = 0;
                loadPage();
            } else {
                return;
            }
        } else {
            section++;
        }
        PRINTF("INFO: ...page %d and section %d\n", this->page, this->section);
        loadSection();
    }
    void MainLayout::prev() {
        PRINTF("INFO: switched from page %d and section %d to...\n", this->page, this->section);
        if(this->section == 0) {
            if(this->page > 1) {
                this->page--;
                this->section = 4;
                loadPage();
            } else {
                return;
            }
        } else {
            section--;
        }
        PRINTF("INFO: ...page %d and section %d\n", this->page, this->section);
        loadSection();
    }
    void MainLayout::loadSection() {
        PRINTF("DEBUG: %d/%d, %d/%d\n", section, maxSection, page, maxPage);
        comicMenu->ClearItems();
        for(short i=0; i<+5; i++) {
            if((i+this->section*5)<comics.size()) addComic(comics[i+this->section*5]);
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
        topStr+=" section: ";
        topStr+=std::to_string(section+1);
        topStr+="/";
        topStr+=std::to_string(maxSection);
        topStr+=")";
        this->topText->SetText(topStr);
        this->topText->SetX(640-(this->topText->GetWidth()/2));
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
        maxSection = comics.size()/5;
        if(this->comics.size()%5) maxSection +=1;
        PRINTF("INFO: loading section %d\n", this->section);
        loadSection();
    }
    void MainLayout::addComic(model::comic Comic) {
        PRINTF("INFO: adding comic %s\n", Comic.id.c_str());;
        MenuItem *item = new MenuItem(Comic.name);
        item->SetColor(theme.textColor);
        item->SetIcon(web::getPath(Comic, 1, true));
        item->SetExtend(Comic.id);
        if(Comic.language != model::CLang::UNKNOWN) item->SetExtendIcon(fs::getFlagPath(Comic.language));
        item->AddOnClick(std::bind(&MainLayout::onItemClick, this));
        this->comicMenu->AddItem(item);
    }
    void MainLayout::showOptions() {
        int opts = mainApp->CreateShowDialog("Category", "what category do you want to see?", {"all", "search", "popular-toggle", "tags (TODO)"}, true);
        if(opts == -1 || opts == -2) return;
        if(opts == 0) {
            mode = model::searchMode::ALL;
        } else if(opts == 1) {
            if(!search()) return;
            this->page = 1;
            this->section = 0;
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
        if(sizeof(tmpstring)>0) return true;
        else return false;
    }
    void MainLayout::onItemClick() {
        comic = this->comics[this->comicMenu->GetSelectedIndex()+5*section];
        mainApp->LoadLayout(mainApp->detailLayout);
        mainApp->detailLayout->showComicDetail();
    }
    void MainLayout::tagSearch() {
        this->mode = model::searchMode::TAG;
        this->page = 1;
        this->section = 0;
        loadPage();
    }
}