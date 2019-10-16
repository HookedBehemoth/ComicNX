#pragma once

#include <pu/Plutonium>

using namespace pu::ui::elm;
namespace ui {
    class DetailLayout : public pu::ui::Layout {
        public:
            DetailLayout();
            PU_SMART_CTOR(DetailLayout)
            void onInput(u64 Down, u64 Up, u64 Held, pu::ui::Touch Pos);
            void showComicDetail();
        private:
            void onItemClick();
            Rectangle::Ref topBarRect;
            Rectangle::Ref focusRect;
            Image::Ref cover;
            Image::Ref logo;
            TextBlock::Ref title;
            TextBlock::Ref pages;
            TextBlock::Ref uploadDate;
            Menu::Ref tagMenu;
    };
}