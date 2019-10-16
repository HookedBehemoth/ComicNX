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
            using Application::Application;
            PU_SMART_CTOR(MainApplication)

            void OnLoad() override;

            void checkPermission();
            MainLayout::Ref mainLayout;
            DetailLayout::Ref detailLayout;
            ImageLayout::Ref imageLayout;
            PCTLFailLayout::Ref pctlFailLayout;
        private:
            void allow();
    };
}