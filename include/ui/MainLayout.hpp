#pragma once

#include <pu/Plutonium>
#include "model.hpp"

using namespace pu::ui::elm;
namespace ui {
    class MainLayout : public pu::ui::Layout {
        public:
            MainLayout();
            ~MainLayout();
            void onInput(u64 Down, u64 Up, u64 Held);
            void loadPage();
            void tagSearch();
        private:
            void next();
            void prev();
            void loadSection();
            void openSelectedComic();
            void addComic(model::comic Comic);
            void showOptions();
            bool search();
            void onItemClick();
            Rectangle *topBarRect;
            Image *logo;
            TextBlock *topText;
            Menu *comicMenu;
            std::vector<model::comic> comics;
            std::string searchString;
            model::searchMode mode = model::searchMode::ALL;
            int page = 1, maxPage = 1, section = 0, maxSection = 0;
            bool popular = false;
    };
}