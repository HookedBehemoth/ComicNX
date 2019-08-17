#pragma once

#include <pu/Plutonium>
#include "comicnx.hpp"
#include "MainLayout.hpp"
#include "DetailLayout.hpp"
#include "ImageLayout.hpp"
#include "web.hpp"

namespace ui {
    class MainApplication : public pu::ui::Application {
        public:
            MainApplication();
            ~MainApplication();
            MainLayout *mainLayout;
            DetailLayout *detailLayout;
            ImageLayout *imageLayout;
    };
}