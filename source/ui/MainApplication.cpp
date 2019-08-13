#include "MainApplication.hpp"

extern model::theme theme;
namespace ui {
    MainApplication *mainApp;

    MainApplication::MainApplication() : pu::ui::Application() {
        mainApp = this;

        this->imageLayout = new ImageLayout();
        this->imageLayout->SetOnInput(std::bind(&MainApplication::image_Input, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        
        this->detailLayout = new DetailLayout();
        this->detailLayout->SetOnInput(std::bind(&MainApplication::detail_Input, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

        this->overviewLayout = new OverviewLayout();
        this->overviewLayout->SetOnInput(std::bind(&MainApplication::overview_Input, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        
        this->LoadLayout(this->overviewLayout);
        this->overviewLayout->catShowAll(1);
    }

    MainApplication::~MainApplication() {
        delete this->detailLayout;
        delete this->imageLayout;
    }

    void MainApplication::image_Input(u64 Down, u64 Up, u64 Held) {
        if(Down & KEY_B) {
            this->LoadLayout(this->detailLayout);
        }
        if((Down & KEY_RIGHT) || (Down & KEY_ZR) || (Down & KEY_R)) {
            this->imageLayout->next();
        }
        if((Down & KEY_LEFT) || (Down & KEY_ZL) || (Down & KEY_L)) {
            this->imageLayout->prev();
        }
    }

    void MainApplication::detail_Input(u64 Down, u64 Up, u64 Held) {
        if((Down & KEY_X) || (Down & KEY_RIGHT) || (Down & KEY_ZR) || (Down & KEY_R)) {
            this->LoadLayout(this->imageLayout);
            this->imageLayout->loadComic();
        }
        if((Down & KEY_LEFT) || (Down & KEY_ZL) || (Down & KEY_L)) {
            this->LoadLayout(this->imageLayout);
            this->imageLayout->loadComicEnd();
        }
        if(Down & KEY_B) {
            this->LoadLayout(this->overviewLayout);
        }
    }

    void MainApplication::overview_Input(u64 Down, u64 Up, u64 Held) {
        if((Down & KEY_PLUS) || (Down & KEY_MINUS)){
            this->Close();
        }
        if(Down & KEY_B) {
            int opt = this->CreateShowDialog("Exit?", "Do you really want to quit?", {"yes", "no"}, true, "romfs:/shrek.png");
            if(opt == 0){
                this->Close();
            }
        }
        if(Down & KEY_X) {
            this->overviewLayout->showOpts();
        }
        if((Down & KEY_RIGHT) || (Down & KEY_ZR) || (Down & KEY_R)) {
            this->overviewLayout->next();
        }
        if((Down & KEY_LEFT) || (Down & KEY_ZL) || (Down & KEY_L)) {
            this->overviewLayout->prev();
        }
    }
}