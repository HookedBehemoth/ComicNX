#pragma once

#include <pu/Plutonium>
#include "MainLayout.hpp"
#include "DetailLayout.hpp"
#include "ImageLayout.hpp"

namespace ui {
    class MainApplication : public pu::ui::Application {
        public:
            MainApplication();
            ~MainApplication();
            MainLayout *mainLayout;
            DetailLayout *detailLayout;
            ImageLayout *imageLayout;
        private:
    };
}