#include "ImageLayout.hpp"
#include "MainApplication.hpp"

extern model::theme theme;
namespace ui {
    extern model::comic comic;
    extern MainApplication *mainApp;

    ImageLayout::ImageLayout() : pu::ui::Layout() {
        this->SetBackgroundColor(theme.background);
        this->image = Image::New(0,0,"romfs:/shrek.png");
        this->image->SetHorizontalAlign(HorizontalAlign::Center);
        this->image->SetVerticalAlign(VerticalAlign::Center);
        this->pageInfo = TextBlock::New(5, 690, "?/?");
        this->pageInfo->SetHorizontalAlign(HorizontalAlign::Left);
        this->pageInfo->SetVerticalAlign(VerticalAlign::Down);
        this->pageInfo->SetColor(theme.textColor);
        this->Add(this->image);
        this->Add(pageInfo);
        this->position = 1;
    }
    void ImageLayout::loadComic() {
        this->position = 1;
        setImage(web::getPath(comic, this->position, false));
    }
    void ImageLayout::loadComicEnd() {
        this->position = comic.pages;
        setImage(web::getPath(comic, this->position, false));
    }
    void ImageLayout::setImage(std::string path) {
        this->image->SetImage(path);
        this->imgHeight = this->image->GetHeight();
        this->imgWidth = this->image->GetWidth();
        fixLayout();
        this->pageInfo->SetText(std::to_string(position) + "/" + std::to_string(comic.pages));
    }
    void ImageLayout::fixLayout() {
        this->image->SetWidth(imgWidth);
        this->image->SetHeight(imgHeight);
        s32 aWSpace = this->wSpace;
        s32 aHSpace = this->hSpace;
        if(isFlipped) {
            aWSpace = this->hSpace;
            aHSpace = this->wSpace;
        }
        if(this->image->GetWidth() > aWSpace) {
            this->image->SetHeight((image->GetHeight()*aWSpace)/this->image->GetWidth());
            this->image->SetWidth(aWSpace);
        }
        if(this->image->GetHeight() > aHSpace) {
            this->image->SetWidth((this->image->GetWidth()*aHSpace)/this->image->GetHeight());
            this->image->SetHeight(aHSpace);
        }

    }
    void ImageLayout::next(){
        if(this->position < comic.pages){
            this->position++;
            setImage(web::getPath(comic, this->position, false));
        } else {
            mainApp->LoadLayout(mainApp->detailLayout);
        }
    }
    void ImageLayout::prev(){
        if(this->position > 1){
            this->position--;
            setImage(web::getPath(comic, this->position, false));
        } else {
            mainApp->LoadLayout(mainApp->detailLayout);
        }
    }
    void ImageLayout::onInput(u64 Down, u64 Up, u64 Held, pu::ui::Touch Pos) {
        if(Down & KEY_B) {
            mainApp->LoadLayout(mainApp->detailLayout);
        }
        if(Down & KEY_X) {
            if(isFlipped) {
                this->image->SetRotation(0);
                isFlipped = false;
            } else {
                this->image->SetRotation(90);
                isFlipped = true;
            }
            fixLayout();
        }
        if((Down & KEY_RIGHT) || (Down & KEY_ZR) || (Down & KEY_R) || (Down & KEY_DOWN)) {
            this->next();
        }
        if((Down & KEY_LEFT) || (Down & KEY_ZL) || (Down & KEY_L) || (Down & KEY_UP)) {
            this->prev();
        }
    }
}