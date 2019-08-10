#include "ImageLayout.hpp"

extern model::theme theme;
namespace ui {
    extern model::comic comic;

    ImageLayout::ImageLayout() : pu::ui::Layout() {
        this->SetBackgroundColor(theme.hoverColor);
        this->image = new Image(0,0,"sdmc:/shrek.png");
        this->Add(this->image);
        position = 1;
    }
    ImageLayout::~ImageLayout() {
        delete this->image;
    }
    void ImageLayout::loadComic() {
        position = 1;
        setImage(web::getPath(comic, position, false));
    }
    void ImageLayout::setImage(std::string path) {
        image->SetImage(path);
        fixLayout();
    }
    void ImageLayout::fixLayout() {
        s32 aWSpace = wSpace - this->image->GetX();
        s32 aHSpace = hSpace - this->image->GetY();
        if(this->image->GetWidth() > aWSpace) {
            this->image->SetHeight((image->GetHeight()*aWSpace)/image->GetWidth());
            this->image->SetWidth(aWSpace);
        }
        if(this->image->GetHeight() > aHSpace) {
            this->image->SetWidth((image->GetWidth()*aHSpace)/image->GetHeight());
            this->image->SetHeight(aHSpace);
        }
        this->image->SetX(640-(image->GetWidth()/2));
        this->image->SetY(360-(image->GetHeight()/2));
    }
    void ImageLayout::next(){
        if(this->position < comic.pages){
            position++;
            setImage(web::getPath(comic, position, false));
        }
    }
    void ImageLayout::prev(){
        if(this->position > 1){
            position--;
            setImage(web::getPath(comic, position, false));
        }
    }
}