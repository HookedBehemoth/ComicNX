#pragma once

#include <pu/Plutonium>
#include "DetailLayout.hpp"
#include "ImageLayout.hpp"
#include "OverviewLayout.hpp"

namespace ui{
    class MainApplication : public pu::ui::Application {
        public:
            MainApplication();
            ~MainApplication();
            DetailLayout *detailLayout;
            ImageLayout *imageLayout;
            OverviewLayout *overviewLayout;
        private:
            void overview_Input(u64 Down, u64 Up, u64 Held);
            void detail_Input(u64 Down, u64 Up, u64 Held);
            void image_Input(u64 Down, u64 Up, u64 Held);
    };
}