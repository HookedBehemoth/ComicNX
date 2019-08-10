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
        if(Down & KEY_RIGHT) {
            this->imageLayout->next();
        }
        if(Down & KEY_LEFT) {
            this->imageLayout->prev();
        }
    }

    void MainApplication::detail_Input(u64 Down, u64 Up, u64 Held) {
        if(Down & KEY_X) {
            this->LoadLayout(this->imageLayout);
            this->imageLayout->loadComic();
        }
        if(Down & KEY_B) {
            this->LoadLayout(this->overviewLayout);
        }
    }

    void MainApplication::overview_Input(u64 Down, u64 Up, u64 Held) {
        if((Down & KEY_PLUS) || (Down & KEY_MINUS)){
            printf("closing\n");
            this->Close();
        }
        if(Down & KEY_B) {
            int opt = this->CreateShowDialog("Exit?", "Do you really want to quit?", {"yes", "no"}, true, "romfs:/shrek.png");
            if(opt == 0){
                printf("closing\n");
                this->Close();
            }
        }
        if(Down & KEY_Y) {
            /*printf("swkbd example\n");
            Result rc=0;
            SwkbdConfig kbd;
            char tmpoutstr[16] = {0};
            printf("swkbdCreate(): 0x%x\n", rc);

            if(R_SUCCEEDED(rc)) {
                swkbdConfigMakePresetDefault(&kbd);
                printf("Running swkbdShow...\n");
                rc = swkbdShow(&kbd, tmpoutstr, sizeof(tmpoutstr));
                printf("swkbdShow(): 0x%x\n", rc);

                if (R_SUCCEEDED(rc)) {
                    printf("out str: %s\n", tmpoutstr);
                }
                swkbdClose(&kbd);
            }*/
            Result rc = 0;
            SwkbdConfig swkbd;
            char tmpstring[256];
            if(R_FAILED(rc = swkbdCreate(&swkbd, 0))){
                swkbdClose(&swkbd);
                return;
            }

            swkbdConfigMakePresetDefault(&swkbd);

            if (R_FAILED(rc = swkbdShow(&swkbd, tmpstring, sizeof(tmpstring)))) {
                swkbdClose(&swkbd);
                return;
	        }

            swkbdClose(&swkbd);

            printf("output: %s\n", tmpstring);
        }
        if(Down & KEY_RIGHT) {
            this->overviewLayout->next();
        }
        if(Down & KEY_LEFT) {
            this->overviewLayout->prev();
        }
    }
}