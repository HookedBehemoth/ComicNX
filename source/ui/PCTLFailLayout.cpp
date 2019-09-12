#include "PCTLFailLayout.hpp"
#include "MainApplication.hpp"

extern model::theme theme;

using namespace pu::ui::elm;
namespace ui {
    extern MainApplication *mainApp;

    PCTLFailLayout::PCTLFailLayout() : pu::ui::Layout() {
        this->SetBackgroundColor(theme.background);
        this->topBarRect = new Rectangle(0, 0, 1280, 100, theme.hoverColor);
        this->focusRect = new Rectangle(40, 120, 1200, 620, theme.hoverColor, 20);
        this->logo = new Image(14, 26, "romfs:/logo.png");
        this->logo->SetWidth(111);
        this->logo->SetHeight(48);
        this->title = new TextBlock(600, 30, "Not permitted", 40);
        this->title->SetColor(theme.textColor);
        this->title->SetX(640-(this->title->GetWidth()/2));
        this->errorText = new TextBlock(600, 625, "Sorry, but you are not permitted to view this content.\nPress B/+ to exit or X to try again!", 40);
        this->errorText->SetColor(theme.textColor);
        this->errorText->SetX(640-(this->errorText->GetWidth()/2));
        this->errorText->SetY(360-(this->errorText->GetHeight()/2));
        this->Add(this->topBarRect);
        this->Add(this->focusRect);
        this->Add(this->logo);
        this->Add(this->title);
        this->Add(this->errorText);
    }
    PCTLFailLayout::~PCTLFailLayout() {
        delete this->logo;
        delete this->topBarRect;
        delete this->focusRect;
        delete this->title;
    }
    void PCTLFailLayout::onInput(u64 Down, u64 Up, u64 Held) {
        if((Down & KEY_B) || (Down & KEY_PLUS)) {
            mainApp->Close();
        }
        if(Down & KEY_X) {
            mainApp->checkPermission();
        }
    }
}