#pragma once

#include <pu/Plutonium>
#include "ui/plutonium/elm_DetailedMenu.hpp"

using namespace pu::ui::elm;
namespace ui {
    class MainLayout : public pu::ui::Layout {
        public:
            MainLayout();
            PU_SMART_CTOR(MainLayout)
            void onInput(u64 Down, u64 Up, u64 Held, pu::ui::Touch Pos);
            void loadPage();
            void tagSearch();
        private:
            void next();
            void prev();
            void displayPage();
            void openSelectedComic();
            void showOptions();
            bool search();
            void onItemClick();
            Rectangle::Ref topBarRect;
            Image::Ref logo;
            TextBlock::Ref topText;
            RichMenu::Ref comicMenu;
            std::vector<nh::Comic> comics;
            std::string searchString;
            enum {
                ALL,
                SEARCH,
                TAG
            } searchMode = ALL;
            int page = 1, maxPage = 1;
            bool popular = false;
    };
}