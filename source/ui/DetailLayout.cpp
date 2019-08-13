#include "DetailLayout.hpp"
#include "MainApplication.hpp"

extern model::theme theme;

using namespace pu::ui::elm;
namespace ui {
    extern MainApplication *mainApp;
    extern model::comic comic;

    DetailLayout::DetailLayout() : pu::ui::Layout() {
        this->cover = new Image(200, 200, "romfs:/shrek.png");
        this->logo = new Image(4, 20, "romfs:/logo.png");
        logo->SetWidth(92);
        logo->SetHeight(60);
        this->SetBackgroundColor(theme.background);
        topBarRect = new Rectangle(0, 0, 1280, 100, theme.hoverColor);
        focusRect = new Rectangle(40, 120, 1200, 600, theme.hoverColor);
        
        this->title = new TextBlock(600, 30, "loading...", 40);
        this->title->SetColor(theme.textColor);
        this->tagMenu = new Menu(640, 130, 590, theme.textColor, 50, 10);
        tagMenu->SetColor(theme.tagBg);
        this->pages = new TextBlock(640, 645, "loading...");
        this->pages->SetColor(theme.textColor);
        this->uploadDate = new TextBlock(640, 675, "loading...");
        this->uploadDate->SetColor(theme.textColor);
        this->Add(this->topBarRect);
        this->Add(this->focusRect);
        this->Add(this->cover);
        this->Add(this->logo);
        this->Add(this->title);
        this->Add(this->tagMenu);
        this->Add(this->pages);
        this->Add(this->uploadDate);
    }
    void DetailLayout::fixStyle() {
        this->title->SetX(640-(this->title->GetTextWidth()/2));
        this->cover->SetX(340-(cover->GetWidth()/2));
        this->cover->SetY(420-(cover->GetHeight()/2));
    }
    DetailLayout::~DetailLayout() {
        delete this->cover;
        delete this->logo;
        delete this->topBarRect;
        delete this->focusRect;
        delete this->title;
        delete this->tagMenu;
        delete this->pages;
        delete this->uploadDate;
    }
    void DetailLayout::showComicDetail() {
        this->tagMenu->SetCooldownEnabled(true);
        this->tagMenu->SetSelectedIndex(0);
        printf("setting local comic...\n");
        printf("setting title...\n");
        title->SetText(comic.name);
        std::string coverPath = "comics/" + comic.id + "/cover" + fs::getSuffix(comic.mediaFType[0]);
        if(!fs::fileExists(coverPath)) {
            printf("making path...\n");
            fs::mkpath("comics/" + comic.id + "/");
            printf("downloading...\n");
            web::downloadFile(
                web::FORMAT_THUMB + comic.mediaId + "/cover" + fs::getSuffix(comic.mediaFType[0]),
                coverPath
                );
        }
        printf("setting cover...\n");
        if(fs::fileExists("sdmc:/switch/" + coverPath))
            this->cover->SetImage(coverPath);

        printf("setting tags...\n");
        this->tagMenu->ClearItems();
        for(auto tag: comic.tags) {
            MenuItem *item = new MenuItem(tag.name);
            item->SetColor(theme.textColor);
            item->AddOnClick(std::bind(&DetailLayout::searchTag, this));
            this->tagMenu->AddItem(item);
        }
        printf("setting misc...\n");
        this->pages->SetText(std::to_string(comic.pages) + " pages");
        this->uploadDate->SetText(utl::getRelativeTime
        (comic.timestamp));
        printf("fix style...\n");
        fixStyle();
    }
    void DetailLayout::debug(){
        this->cover->SetX(0);
        this->cover->SetY(0);
    }
    void DetailLayout::searchTag(){
        model::tag tag = comic.tags[this->tagMenu->GetSelectedIndex()];
        printf("clicked on %s\n", tag.name.c_str());
        int opts = mainApp->CreateShowDialog("search Tag:" , "do you really want to search for: " + tag.name, {"OK", "Cancel"}, true);
        if(opts == 0) {
            printf("searching for tag: %s: %d\n", tag.name.c_str(), std::to_string(tag.id));
            mainApp->overviewLayout->catShowTagged(tag, 1);
            mainApp->LoadLayout(mainApp->overviewLayout);
        }
    }
}