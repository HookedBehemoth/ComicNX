#pragma once

#include <pu/Plutonium>

using namespace pu::ui::elm;
namespace ui {
    class DetailLayout : public pu::ui::Layout {
        public:
            DetailLayout();
            ~DetailLayout();
            void onInput(u64 Down, u64 Up, u64 Held);
            void showComicDetail();
        private:
            void onItemClick();
            Rectangle *topBarRect;
            Rectangle *focusRect;
            Image *cover;
            Image *logo;
            TextBlock *title;
            TextBlock *pages;
            TextBlock *uploadDate;
            Menu *tagMenu;
    };
}