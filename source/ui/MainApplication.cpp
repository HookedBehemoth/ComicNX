#include "MainApplication.hpp"
#include "model.hpp"

extern model::theme theme;
namespace ui {
    MainApplication *mainApp;

    MainApplication::MainApplication() : pu::ui::Application() {
        mainApp = this;

        this->mainLayout = new MainLayout();
        this->mainLayout->SetOnInput(std::bind(&MainLayout::onInput, this->mainLayout, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        this->detailLayout = new DetailLayout();
        this->detailLayout->SetOnInput(std::bind(&DetailLayout::onInput, this->detailLayout, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        this->imageLayout = new ImageLayout();
        this->imageLayout->SetOnInput(std::bind(&ImageLayout::onInput, this->imageLayout, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        this->pctlFailLayout = new PCTLFailLayout();
        this->pctlFailLayout->SetOnInput(std::bind(&PCTLFailLayout::onInput, this->pctlFailLayout, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

        checkPermission();
    }

    MainApplication::~MainApplication() {
        delete(this->mainLayout);
    }

    void MainApplication::checkPermission() {
        pctlInitialize();
        Result rc;
        bool pcactive;
        rc = pctlIsRestrictionEnabled(&pcactive);
        if(R_SUCCEEDED(rc) && pcactive) {
            rc = pctlauthShow(true);
            if(R_SUCCEEDED(rc)) {
                allow();
            } else {
                this->LoadLayout(this->pctlFailLayout);
            }
        } else {
            allow();
        }
        printf("rc: %d\npcactive: %b", rc, pcactive);
        pctlExit();
    }

    void MainApplication::allow() {
        PRINTF("INFO: loading layout: mainLayout\n");
        this->LoadLayout(this->mainLayout);
        PRINTF("INFO: loading page\n");
        this->mainLayout->loadPage();
    }
}