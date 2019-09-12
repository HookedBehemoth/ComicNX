#pragma once

#include <pu/Plutonium>
#include "comicnx.hpp"
#include "MainLayout.hpp"
#include "DetailLayout.hpp"
#include "ImageLayout.hpp"
#include "PCTLFailLayout.hpp"
#include "web.hpp"

namespace ui {
    class MainApplication : public pu::ui::Application {
        public:
            MainApplication();
            ~MainApplication();
            void checkPermission();
            MainLayout *mainLayout;
            DetailLayout *detailLayout;
            ImageLayout *imageLayout;
            PCTLFailLayout *pctlFailLayout;
        private:
            void allow();
    };
}