#pragma once

#include <pu/Plutonium>
#include "web.hpp"
using namespace pu::ui::elm;
namespace ui{    
    class ImageLayout : public pu::ui::Layout {
        public:
            ImageLayout();
            ~ImageLayout();
            void loadComic();
            void next();
            void prev();
        private:
            const s32 wSpace = 1280;
            const s32 hSpace = 720;
            int position;
            void setImage(std::string path);
            void fixLayout();
            Image *image;
    };
}