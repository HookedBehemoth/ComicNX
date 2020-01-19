#include "PCTLFailLayout.hpp"
#include "MainApplication.hpp"
#include "nh/theme.hpp"

using namespace pu::ui::elm;
namespace ui {
    extern MainApplication *mainApp;

    PCTLFailLayout::PCTLFailLayout() : pu::ui::Layout() {
        this->SetBackgroundColor(theme::back);
        this->topBarRect = Rectangle::New(0, 0, 1280, 100, theme::hover);
        this->focusRect = Rectangle::New(40, 120, 1200, 620, theme::hover, 20);
        this->logo = Image::New(14, 26, "romfs:/logo.png");
        this->logo->SetWidth(111);
        this->logo->SetHeight(48);
        this->title = TextBlock::New(600, 30, "Not permitted", 40);
        this->title->SetColor(theme::text);
        this->title->SetX(640-(this->title->GetWidth()/2));
        this->errorText = TextBlock::New(600, 625, "Sorry, but you are not permitted to view this content.\nPress B/+ to exit or X to try again!", 40);
        this->errorText->SetColor(theme::text);
        this->errorText->SetX(640-(this->errorText->GetWidth()/2));
        this->errorText->SetY(360-(this->errorText->GetHeight()/2));
        this->Add(this->topBarRect);
        this->Add(this->focusRect);
        this->Add(this->logo);
        this->Add(this->title);
        this->Add(this->errorText);
    }
    
    void PCTLFailLayout::onInput(u64 Down, u64 Up, u64 Held, pu::ui::Touch Pos) {
        if((Down & KEY_B) || (Down & KEY_PLUS)) {
            mainApp->Close();
        }
        if(Down & KEY_X) {
            mainApp->checkPermission();
        }
    }
}