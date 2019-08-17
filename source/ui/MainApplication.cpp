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
        PRINTF("INFO: loading layout: mainLayout\n");
        this->LoadLayout(this->mainLayout);
        PRINTF("INFO: loading page\n");
        this->mainLayout->loadPage();
    }

    MainApplication::~MainApplication() {
        delete(this->mainLayout);
    }
}