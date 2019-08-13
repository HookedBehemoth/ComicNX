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
            void catShowSearch(std::string search, int page);
            void catShowTagged(model::tag tag, int page);
            void loadComics();
            void showOpts();
            void search();
            void next();
            void prev();
            bool popular = false;
        private:
            void loadFromLink(std::string url);
            void showPage(int i);
            void onClick();
            Menu *comicMenu;
            Rectangle *topBarRect;
            Image *logo;
            TextBlock *pageInfo;
            std::string searchString;
            std::vector<model::comic> comics;
            model::tag currentTag;
            int page,
                maxPage,
                mode;
    };
}