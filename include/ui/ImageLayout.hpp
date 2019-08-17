#pragma once

#include <pu/Plutonium>

using namespace pu::ui::elm;
namespace ui {
    class ImageLayout : public pu::ui::Layout {
        public:
            ImageLayout();
            ~ImageLayout();
            void loadComic();
            void loadComicEnd();
            void onInput(u64 Down, u64 Up, u64 Held);
            void next();
            void prev();
        private:
            const s32 wSpace = 1280;
            const s32 hSpace = 720;
            int position;
            void setImage(std::string path);
            void fixLayout();
            Image *image;
            TextBlock *pageInfo;
    };
}