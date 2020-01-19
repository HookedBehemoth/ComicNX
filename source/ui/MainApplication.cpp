#include "MainApplication.hpp"

namespace ui {
    MainApplication *mainApp;

    void MainApplication::OnLoad() {
        mainApp = this;

        this->mainLayout = MainLayout::New();
        this->mainLayout->SetOnInput(std::bind(&MainLayout::onInput, this->mainLayout, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
        this->detailLayout = DetailLayout::New();
        this->detailLayout->SetOnInput(std::bind(&DetailLayout::onInput, this->detailLayout, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
        this->imageLayout = ImageLayout::New();
        this->imageLayout->SetOnInput(std::bind(&ImageLayout::onInput, this->imageLayout, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
        this->pctlFailLayout = PCTLFailLayout::New();
        this->pctlFailLayout->SetOnInput(std::bind(&PCTLFailLayout::onInput, this->pctlFailLayout, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

        checkPermission();
    }
    
    void MainApplication::checkPermission() {
        pctlInitialize();
        Result rc;
        bool pcactive = false;
        rc = pctlIsRestrictionEnabled(&pcactive);
        pctlExit();
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
        printf("rc: 0x%x\npcactive: %s\n", rc, pcactive ? "true" : "false");
    }

    void MainApplication::allow() {
        printf("INFO: loading layout: mainLayout\n");
        this->LoadLayout(this->mainLayout);
        printf("INFO: loading page\n");
        this->mainLayout->loadPage();
    }
}