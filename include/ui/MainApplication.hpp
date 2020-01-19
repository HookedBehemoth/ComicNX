#pragma once

#include <pu/Plutonium>
#include "ui/MainLayout.hpp"
#include "ui/DetailLayout.hpp"
#include "ui/ImageLayout.hpp"
#include "ui/PCTLFailLayout.hpp"

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