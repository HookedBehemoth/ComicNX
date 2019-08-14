#include "ImageLayout.hpp"
#include "MainApplication.hpp"

extern model::theme theme;
namespace ui {
    extern model::comic comic;
    extern MainApplication *mainApp;

    ImageLayout::ImageLayout() : pu::ui::Layout() {
        this->SetBackgroundColor(theme.background);
        this->image = new Image(0,0,"romfs:/shrek.png");
        this->pageInfo = new TextBlock(5, 690, "?/?");
        this->pageInfo->SetColor(theme.textColor);
        this->Add(this->image);
        this->Add(pageInfo);
        this->position = 1;
    }
    ImageLayout::~ImageLayout() {
        delete this->image;
        delete this->pageInfo;
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
        fixLayout();
        this->pageInfo->SetText(std::to_string(position) + "/" + std::to_string(comic.pages));
    }
    void ImageLayout::fixLayout() {
        s32 aWSpace = this->wSpace - this->image->GetX();
        s32 aHSpace = this->hSpace - this->image->GetY();
        if(this->image->GetWidth() > aWSpace) {
            this->image->SetHeight((image->GetHeight()*aWSpace)/this->image->GetWidth());
            this->image->SetWidth(aWSpace);
        }
        if(this->image->GetHeight() > aHSpace) {
            this->image->SetWidth((this->image->GetWidth()*aHSpace)/this->image->GetHeight());
            this->image->SetHeight(aHSpace);
        }
        this->image->SetX(640-(this->image->GetWidth()/2));
        this->image->SetY(360-(this->image->GetHeight()/2));
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
}