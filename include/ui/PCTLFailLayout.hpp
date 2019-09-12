#pragma once

#include <pu/Plutonium>

using namespace pu::ui::elm;
namespace ui {
    class PCTLFailLayout : public pu::ui::Layout {
        public:
            PCTLFailLayout();
            ~PCTLFailLayout();
            void onInput(u64 Down, u64 Up, u64 Held);
        private:
            Rectangle *topBarRect;
            Rectangle *focusRect;
            Image *logo;
            TextBlock *title;
            TextBlock *errorText;
    };
}