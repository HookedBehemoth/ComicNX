#include "DetailLayout.hpp"
#include "MainApplication.hpp"
#include "utl.hpp"

extern model::theme theme;

using namespace pu::ui::elm;
namespace ui {
    extern MainApplication *mainApp;
    extern model::tag searchTag;
    extern model::comic comic;

    DetailLayout::DetailLayout() : pu::ui::Layout() {
        this->SetBackgroundColor(theme.background);
        this->topBarRect = Rectangle::New(0, 0, 1280, 100, theme.hoverColor);
        this->focusRect = Rectangle::New(40, 120, 1200, 620, theme.hoverColor, 20);
        this->cover = Image::New(200, 200, "romfs:/shrek.png");
        this->logo = Image::New(14, 26, "romfs:/logo.png");
        this->logo->SetWidth(111);
        this->logo->SetHeight(48);
        this->title = TextBlock::New(600, 30, "loading...", 40);
        this->title->SetHorizontalAlign(HorizontalAlign::Center);
        this->title->SetVerticalAlign(VerticalAlign::Up);
        this->title->SetColor(theme.textColor);
        this->tagMenu = Menu::New(640, 130, 590, theme.textColor, 50, 10);
        this->tagMenu->SetColor(theme.tagBg);
        this->pages = TextBlock::New(640, 645, "loading...");
        this->pages->SetColor(theme.textColor);
        this->uploadDate = TextBlock::New(640, 675, "loading...");
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
    
    void DetailLayout::onInput(u64 Down, u64 Up, u64 Held, pu::ui::Touch Pos) {
        if((Down & KEY_X) || (Down & KEY_RIGHT) || (Down & KEY_ZR) || (Down & KEY_R)) {
            mainApp->LoadLayout(mainApp->imageLayout);
            mainApp->imageLayout->loadComic();
        }
        if((Down & KEY_LEFT) || (Down & KEY_ZL) || (Down & KEY_L)) {
            mainApp->LoadLayout(mainApp->imageLayout);
            mainApp->imageLayout->loadComicEnd();
        }
        if(Down & KEY_B) {
            mainApp->LoadLayout(mainApp->mainLayout);
        }
    }
    void DetailLayout::showComicDetail() {
        this->tagMenu->SetCooldownEnabled(true);
        this->tagMenu->SetSelectedIndex(0);
        PRINTF("INFO: setting local comic...\n");
        this->title->SetText(comic.name);
        this->cover->SetImage(web::getPath(comic, 1, false));
        this->tagMenu->ClearItems();
        for(auto tag: comic.tags) {
            MenuItem::Ref item = MenuItem::New(tag.name);
            item->SetColor(theme.textColor);
            if(searchTag==tag) item->SetColor(theme.background);
            item->AddOnClick(std::bind(&DetailLayout::onItemClick, this));
            this->tagMenu->AddItem(item);
        }
        this->pages->SetText(std::to_string(comic.pages) + " pages");
        this->uploadDate->SetText(utl::getRelativeTime(comic.timestamp));
        this->title->SetX(640-(this->title->GetTextWidth()/2));
        if(this->cover->GetWidth() > 600) {
            this->cover->SetHeight((cover->GetHeight()*600)/this->cover->GetWidth());
            this->cover->SetWidth(600);
        }
        if(this->cover->GetHeight() > 600) {
            this->cover->SetWidth((this->cover->GetWidth()*600)/this->cover->GetHeight());
            this->cover->SetHeight(600);
        }
        this->cover->SetX(340-(cover->GetWidth()/2));
        this->cover->SetY(420-(cover->GetHeight()/2));
    }
    void DetailLayout::onItemClick() {
        model::tag tag = comic.tags[this->tagMenu->GetSelectedIndex()];
        PRINTF("INFO: clicked on %s\n", tag.name.c_str());
        int opts = mainApp->CreateShowDialog("search Tag?" , "do you really want to search for: " + tag.name, {"OK", "Cancel"}, true);
        if(opts == 0) {
            PRINTF("INFO: searching for tag: %s: %d\n", tag.name.c_str(), tag.id);
            mainApp->LoadLayout(mainApp->mainLayout);
            if(!(searchTag==tag)) {
                searchTag = tag;
                mainApp->mainLayout->tagSearch();
            }
        }
    }
}