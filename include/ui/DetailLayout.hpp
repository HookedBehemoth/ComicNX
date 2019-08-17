#pragma once

#include <pu/Plutonium>
#include "web.hpp"
#include "utl.hpp"

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
            Rectangle   *topBarRect,
                        *focusRect;
            Image *cover, *logo;
            TextBlock   *title,
                        *pages,
                        *uploadDate;
            Menu *tagMenu;
    };
}