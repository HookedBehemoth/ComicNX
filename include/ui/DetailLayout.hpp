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
            void showComicDetail();
            void debug();
        private:
            void searchTag();
            void fixStyle();
            Image *cover, *logo;
            TextBlock   *title,
                        *pages,
                        *uploadDate;
            Menu *tagMenu;
            Rectangle   *topBarRect,
                        *focusRect;
    };
}