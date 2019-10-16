#pragma once

#include <pu/Plutonium>

using namespace pu::ui::elm;
namespace ui {
    class PCTLFailLayout : public pu::ui::Layout {
        public:
            PCTLFailLayout();
            PU_SMART_CTOR(PCTLFailLayout)
            void onInput(u64 Down, u64 Up, u64 Held, pu::ui::Touch Pos);
        private:
            Rectangle::Ref topBarRect;
            Rectangle::Ref focusRect;
            Image::Ref logo;
            TextBlock::Ref title;
            TextBlock::Ref errorText;
    };
}