
/*

    Plutonium library

    @file Menu.hpp
    @brief A Menu is a very useful Element for option browsing or selecting.
    @author XorTroll

    @copyright Plutonium project - an easy-to-use UI framework for Nintendo Switch homebrew

*/

#pragma once
#include <pu/ui/elm/elm_Element.hpp>
#include <vector>
#include <chrono>
#include <functional>
#include "nh/comic.hpp"

namespace pu::ui::elm
{
    class RichMenu : public Element
    {
        public:
            RichMenu(size_t X, size_t Y, size_t Width, Color OptionColor, size_t ItemSize, size_t ItemsToShow);
            PU_SMART_CTOR(RichMenu)

            s32 GetX();
            void SetX(size_t X);
            s32 GetY();
            void SetY(size_t Y);
            s32 GetWidth();
            void SetWidth(size_t Width);
            s32 GetHeight();
            size_t GetItemSize();
            void SetItemSize(size_t ItemSize);
            size_t GetNumberOfItemsToShow();
            void SetNumberOfItemsToShow(size_t ItemsToShow);
            Color GetColor();
            void SetColor(Color Color);
            Color GetOnFocusColor();
            void SetOnFocusColor(Color Color);
            Color GetScrollbarColor();
            void SetScrollbarColor(Color Color);
            void SetCallback(std::function<void()> Callback);
            void SetOnSelectionChanged(std::function<void()> Callback);
            void AddItem(const nh::Comic& comic);
            void ClearItems();
            void SetCooldownEnabled(bool Cooldown);
            const nh::Comic& GetSelectedItem();
            size_t GetSelectedIndex();
            void SetSelectedIndex(size_t Index);
            void OnRender(render::Renderer::Ref &Drawer, s32 X, s32 Y);
            void OnInput(u64 Down, u64 Up, u64 Held, Touch Pos);
        private:
            void ReloadItemRenders();
            bool dtouch;
            size_t x;
            size_t y;
            size_t w;
            size_t isize;
            size_t ishow;
            size_t previsel;
            size_t fisel;
            size_t isel;
            size_t pselfact;
            size_t selfact;
            Color scb;
            Color clr;
            Color fcs;
            bool icdown;
            int basestatus;
            std::chrono::time_point<std::chrono::steady_clock> basetime;
            std::function<void()> onclick;
            std::function<void()> onselch;
            std::vector<nh::Comic> itms;
            render::NativeFont basefont;
            render::NativeFont richfont;
            render::NativeFont basefont_meme;
            render::NativeFont richfont_meme;
            std::vector<render::NativeTexture> loadednames;
            std::vector<render::NativeTexture> loadedicons;
            std::vector<render::NativeTexture> loadedrichnames;
            std::vector<render::NativeTexture> loadedrichicons;
    };
}