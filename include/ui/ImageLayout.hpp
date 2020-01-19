#pragma once

#include <pu/Plutonium>

using namespace pu::ui::elm;
namespace ui {
    class ImageLayout : public pu::ui::Layout {
        public:
            ImageLayout();
            PU_SMART_CTOR(ImageLayout)
            void loadComic();
            void loadComicEnd();
            void onInput(u64 Down, u64 Up, u64 Held, pu::ui::Touch Pos);
            void next();
            void prev();
        private:
            const s32 wSpace = 1280;
            const s32 hSpace = 720;
            s32 imgWidth;
            s32 imgHeight;
            int position;
            bool isFlipped = false;
            void showImage();
            void fixLayout();
            Image::Ref image;
            TextBlock::Ref pageInfo;
    };
}