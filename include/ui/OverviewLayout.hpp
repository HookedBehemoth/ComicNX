#pragma once

#include <pu/Plutonium>
#include "web.hpp"

using namespace pu::ui::elm;
namespace ui {
    class OverviewLayout : public pu::ui::Layout {
        public:
            OverviewLayout();
            ~OverviewLayout();
            void catShowAll(int page);
            void catShowTagged(int tag, int page);
            void catShowSearch(std::string search, int page);
            void loadComics();
            void showOpts();
            void search();
            void next();
            void prev();
        private:
            Menu *comicMenu;
            void loadFromLink(std::string url);
            std::vector<model::comic> comics;
            void onClick();
            Rectangle *topBarRect;
            Image *logo;
            std::string searchString;
            model::tag currentTag;
            int page, maxPage, mode;
            void showPage(int i);
    };
}