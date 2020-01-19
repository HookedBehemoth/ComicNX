#include "ui/plutonium/elm_DetailedMenu.hpp"
#include "nh/theme.hpp"

namespace pu::ui::elm
{
    RichMenu::RichMenu(size_t X, size_t Y, size_t Width, Color OptionColor, size_t ItemSize, size_t ItemsToShow) : Element::Element()
    {
        this->x = X;
        this->y = Y;
        this->w = Width;
        this->clr = OptionColor;
        this->scb = { 110, 110, 110, 255 };
        this->isize = ItemSize;
        this->ishow = ItemsToShow;
        this->previsel = 0;
        this->isel = 0;
        this->fisel = 0;
        this->selfact = 255;
        this->pselfact = 0;
        this->onselch = [&](){};
        this->icdown = false;
        this->dtouch = false;
        this->fcs = { 40, 40, 40, 255 };
        this->basestatus = 0;
        this->basefont = render::LoadDefaultFont(ItemSize/3);
        this->richfont = render::LoadDefaultFont(ItemSize/4);
        this->basefont_meme = render::LoadSharedFont(pu::ui::render::SharedFont::NintendoExtended ,ItemSize/3);
        this->richfont_meme = render::LoadSharedFont(pu::ui::render::SharedFont::NintendoExtended ,ItemSize/4);
    }

    s32 RichMenu::GetX()
    {
        return this->x;
    }

    void RichMenu::SetX(size_t X)
    {
        this->x = X;
    }

    s32 RichMenu::GetY()
    {
        return this->y;
    }

    void RichMenu::SetY(size_t Y)
    {
        this->y = Y;
    }

    s32 RichMenu::GetWidth()
    {
        return this->w;
    }

    void RichMenu::SetWidth(size_t Width)
    {
        this->w = Width;
    }

    s32 RichMenu::GetHeight()
    {
        return (this->isize * this->ishow);
    }

    size_t RichMenu::GetItemSize()
    {
        return this->isize;
    }

    void RichMenu::SetItemSize(size_t ItemSize)
    {
        this->isize = ItemSize;
    }

    size_t RichMenu::GetNumberOfItemsToShow()
    {
        return this->ishow;
    }

    void RichMenu::SetNumberOfItemsToShow(size_t ItemsToShow)
    {
        this->ishow = ItemsToShow;
    }

    Color RichMenu::GetColor()
    {
        return this->clr;
    }

    void RichMenu::SetColor(Color Color)
    {
        this->clr = Color;
    }
    
    Color RichMenu::GetOnFocusColor()
    {
        return this->fcs;
    }

    void RichMenu::SetOnFocusColor(Color Color)
    {
        this->fcs = Color;
    }

    Color RichMenu::GetScrollbarColor()
    {
        return this->scb;
    }

    void RichMenu::SetScrollbarColor(Color Color)
    {
        this->scb = Color;
    }

    void RichMenu::SetCallback(std::function<void()> Callback)
    {
        this->onclick = Callback;
    }

    void RichMenu::SetOnSelectionChanged(std::function<void()> Callback)
    {
        this->onselch = Callback;
    }

    void RichMenu::AddItem(const nh::Comic& comic)
    {
        this->itms.push_back(comic);
    }

    void RichMenu::ClearItems()
    {
        this->itms.clear();
        this->loadednames.clear();
        this->loadedicons.clear();
        this->loadedrichnames.clear();
        this->loadedrichicons.clear();
    }

    void RichMenu::SetCooldownEnabled(bool Cooldown)
    {
        this->icdown = Cooldown;
    }

    const nh::Comic& RichMenu::GetSelectedItem()
    {
        return this->itms[this->isel];
    }

    size_t RichMenu::GetSelectedIndex()
    {
        return this->isel;
    }

    void RichMenu::SetSelectedIndex(size_t Index)
    {
        if(this->itms.size() > Index)
        {
            this->isel = Index;
            this->fisel = 0;
            if(this->isel > 0)
            {
                u32 div = (this->itms.size() + this->ishow - 1) / this->ishow;
                for(u32 i = 0; i < div; i++)
                {
                    if((this->ishow * i) > this->isel)
                    {
                        this->fisel = this->ishow * (i - 1);
                        break;
                    }
                }
            }
            ReloadItemRenders();
            this->selfact = 255;
            this->pselfact = 0;
        }
    }

    void RichMenu::OnRender(render::Renderer::Ref &Drawer, s32 X, s32 Y)
    {
        if(!this->itms.empty())
        {
            size_t cx = X;
            size_t cy = Y;
            size_t cw = this->w;
            size_t ch = this->isize;
            size_t its = this->ishow;
            if(its > this->itms.size()) its = this->itms.size();
            if((its + this->fisel) > this->itms.size()) its = this->itms.size() - this->fisel;
            if(this->loadednames.empty()) ReloadItemRenders();
            for(size_t i = this->fisel; i < (its + this->fisel); i++)
            {
                size_t clrr = this->clr.R;
                size_t clrg = this->clr.G;
                size_t clrb = this->clr.B;
                size_t nr = clrr - 70;
                if(nr < 0) nr = 0;
                size_t ng = clrg - 70;
                if(ng < 0) ng = 0;
                size_t nb = clrb - 70;
                if(nb < 0) nb = 0;
                Color nclr(nr, ng, nb, this->clr.A);
                auto loadedidx = i - this->fisel;
                auto curname = this->loadednames[loadedidx];
                auto curicon = this->loadedicons[loadedidx];
                auto currichname = this->loadedrichnames[loadedidx];
                auto currichicon = this->loadedrichicons[loadedidx];
                if(this->isel == i)
                {
                    Drawer->RenderRectangleFill(this->clr, cx, cy, cw, ch);
                    if(this->selfact < 255)
                    {
                        Drawer->RenderRectangleFill(Color(this->fcs.R, this->fcs.G, this->fcs.B, this->selfact), cx, cy, cw, ch);
                        this->selfact += 48;
                    }
                    else Drawer->RenderRectangleFill(this->fcs, cx, cy, cw, ch);
                }
                else if(this->previsel == i)
                {
                    Drawer->RenderRectangleFill(this->clr, cx, cy, cw, ch);
                    if(this->pselfact > 0)
                    {
                        Drawer->RenderRectangleFill(Color(this->fcs.R, this->fcs.G, this->fcs.B, this->pselfact), cx, cy, cw, ch);
                        this->pselfact -= 48;
                    }
                    else Drawer->RenderRectangleFill(this->clr, cx, cy, cw, ch);
                }
                else Drawer->RenderRectangleFill(this->clr, cx, cy, cw, ch);
                const nh::Comic& itm = this->itms[i];
                size_t xh = render::GetTextureHeight(curname);
                size_t tx = (cx + 25);
                size_t ty;
                if(itm.id == 0) ty = ((ch - xh) / 2) + cy;
                else ty = (ch/3) - (xh/2) + cy;
                if(itm.mediaId != 0)
                {
                    float factor = (float)render::GetTextureHeight(curicon)/(float)render::GetTextureWidth(curicon);
                    size_t icw = (this->isize - 10);
                    size_t ich = icw;
                    size_t icx = (cx + 25);
                    size_t icy = (cy + 5);
                    tx = (icx + icw + 25);
                    if(factor < 1)
                    {
                        ich = ich*factor;
                        icy = icy+((this->isize-ich)/2);
                    } else
                    {
                        icw = icw/factor;
                        icx = icx+((this->isize-icw)/2);
                    }
                    Drawer->RenderTexture(curicon, icx, icy, { -1, (s32)icw, (s32)ich, -1.0f });
                }
                if(itm.id)
                {
                    size_t rxh = render::GetTextureHeight(currichname);
                    size_t rtx = tx;
                    size_t rty = cy + (ch/3)*2 + ((ch/3) - rxh)/2;
                    if(itm.getLanguage() != nh::Language::UNKNOWN)
                    {
                        float rfactor = (float)render::GetTextureHeight(currichicon)/(float)render::GetTextureWidth(currichicon);
                        size_t ricw = (ch/3);
                        size_t rich = ricw;
                        size_t ricx = rtx;
                        size_t ricy = (cy + (ch/3)*2);
                        if(rfactor < 1)
                        {
                            rich = rich*rfactor;
                            ricy = ricy+((ricw-rich)/2);
                        }
                        else
                        {
                            ricw = ricw/rfactor;
                            ricx = ricx+((rich-ricw)/2);
                        }
                        rtx = ricx + ricw + 10;
                        Drawer->RenderTexture(currichicon, ricx, ricy, { -1, (s32)ricw, (s32)rich, -1.0f });
                    }
                    Drawer->RenderTexture(currichname, rtx, rty);
                }
                Drawer->RenderTexture(curname, tx, ty);
                cy += ch;
            }
            if(this->ishow < this->itms.size())
            {
                size_t sccr = this->scb.R;
                size_t sccg = this->scb.G;
                size_t sccb = this->scb.B;
                size_t snr = sccr - 30;
                if(snr < 0) snr = 0;
                size_t sng = sccg - 30;
                if(sng < 0) sng = 0;
                size_t snb = sccb - 30;
                if(snb < 0) snb = 0;
                Color sclr(snr, sng, snb, this->scb.A);
                size_t scx = this->x + (this->w - 20);
                size_t scy = this->y;
                size_t scw = 20;
                size_t sch = (this->ishow * this->isize);
                Drawer->RenderRectangleFill(this->scb, scx, scy, scw, sch);
                size_t fch = ((this->ishow * sch) / this->itms.size());
                size_t fcy = scy + (this->fisel * (sch / this->itms.size()));
                Drawer->RenderRectangleFill(sclr, scx, fcy, scw, fch);
            }
            Drawer->RenderShadowSimple(cx, cy, cw, 5, 160);
        }
    }

    void RichMenu::OnInput(u64 Down, u64 Up, u64 Held, Touch Pos)
    {
        if(!this->itms.empty())
        {
            if(basestatus == 1)
            {
                auto curtime = std::chrono::steady_clock::now();
                auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(curtime - basetime).count();
                if(diff >= 150)
                {
                    basestatus = 2;
                }
            }
            if(!Pos.IsEmpty())
            {
                size_t cx = this->x;
                size_t cy = this->y;
                size_t cw = this->w;
                size_t ch = this->isize;
                size_t its = this->ishow;
                if(its > this->itms.size()) its = this->itms.size();
                if((its + this->fisel) > this->itms.size()) its = this->itms.size() - this->fisel;
                for(size_t i = this->fisel; i < (this->fisel + its); i++)
                {
                    if(((s32)(cx + cw) > Pos.X) && (Pos.X > (s32)cx) && ((s32)(cy + ch) > Pos.Y) && (Pos.Y > (s32)cy))
                    {
                        this->dtouch = true;
                        this->previsel = this->isel;
                        this->isel = i;
                        (this->onselch)();
                        if(i == this->isel) this->selfact = 255;
                        else if(i == this->previsel) this->pselfact = 0;
                        break;
                    }
                    cy += this->isize;
                }
            }
            else if(this->dtouch)
            {
                if((this->selfact >= 255) && (this->pselfact <= 0))
                {
                    if(this->icdown) this->icdown = false;
                    else (this->onclick)();
                    this->dtouch = false;
                }
            }
            else
            {
                if((Down & KEY_DDOWN) || (Down & KEY_LSTICK_DOWN) || (Held & KEY_RSTICK_DOWN))
                {
                    bool move = true;
                    if(Held & KEY_RSTICK_DOWN)
                    {
                        move = false;
                        if(basestatus == 0)
                        {
                            basetime = std::chrono::steady_clock::now();
                            basestatus = 1;
                        }
                        else if(basestatus == 2)
                        {
                            basestatus = 0;
                            move = true;
                        }
                    }
                    if(move)
                    {
                        if(this->isel < (this->itms.size() - 1))
                        {
                            if((this->isel - this->fisel) == (this->ishow - 1))
                            {
                                this->fisel++;
                                this->isel++;
                                (this->onselch)();
                                ReloadItemRenders();
                            }
                            else
                            {
                                this->previsel = this->isel;
                                this->isel++;
                                (this->onselch)();
                                if(!this->itms.empty()) for(size_t i = 0; i < this->itms.size(); i++)
                                {
                                    if(i == this->isel) this->selfact = 0;
                                    else if(i == this->previsel) this->pselfact = 255;
                                }
                                ReloadItemRenders();
                            }
                        }
                        else
                        {
                            this->isel = 0;
                            this->fisel = 0;
                            ReloadItemRenders();
                        }
                    }
                }
                else if((Down & KEY_DUP) || (Down & KEY_LSTICK_UP) || (Held & KEY_RSTICK_UP))
                {
                    bool move = true;
                    if(Held & KEY_RSTICK_UP)
                    {
                        move = false;
                        if(basestatus == 0)
                        {
                            basetime = std::chrono::steady_clock::now();
                            basestatus = 1;
                        }
                        else if(basestatus == 2)
                        {
                            basestatus = 0;
                            move = true;
                        }
                    }
                    if(move)
                    {
                        if(this->isel > 0)
                        {
                            if(this->isel == this->fisel)
                            {
                                this->fisel--;
                                this->isel--;
                                (this->onselch)();
                                ReloadItemRenders();
                            }
                            else
                            {
                                this->previsel = this->isel;
                                this->isel--;
                                (this->onselch)();
                                if(!this->itms.empty()) for(size_t i = 0; i < this->itms.size(); i++)
                                {
                                    if(i == this->isel) this->selfact = 0;
                                    else if(i == this->previsel) this->pselfact = 255;
                                }
                                ReloadItemRenders();
                            }
                        }
                        else
                        {
                            this->isel = this->itms.size() - 1;
                            this->fisel = 0;
                            if(this->itms.size() >= this->ishow) this->fisel = this->itms.size() - this->ishow;
                            ReloadItemRenders();
                        }
                    }
                }
                else
                {
                        if(Down & KEY_A)
                        {
                            if(this->icdown) this->icdown = false;
                            else (this->onclick)();
                        }
                }
            }
        }
    }
    
    void RichMenu::ReloadItemRenders()
    {
        for(u32 i = 0; i < this->loadednames.size(); i++) render::DeleteTexture(this->loadednames[i]);
        for(u32 i = 0; i < this->loadedicons.size(); i++) render::DeleteTexture(this->loadedicons[i]);
        for(u32 i = 0; i < this->loadedrichnames.size(); i++) render::DeleteTexture(this->loadedrichnames[i]);
        for(u32 i = 0; i < this->loadedrichicons.size(); i++) render::DeleteTexture(this->loadedrichnames[i]);
        this->loadednames.clear();
        this->loadedicons.clear();
        this->loadedrichnames.clear();
        this->loadedrichicons.clear();
        size_t its = this->ishow;
        if(its > this->itms.size()) its = this->itms.size();
        if((its + this->fisel) > this->itms.size()) its = this->itms.size() - this->fisel;
        for(size_t i = this->fisel; i < (its + this->fisel); i++)
        {
            /* name */
            auto strname = this->itms[i].toString();
            auto tex = render::RenderText(this->basefont, this->basefont_meme, strname, theme::text);
            this->loadednames.push_back(tex);
            /* icon */
            auto img = itms[i].loadPreview();
            if (img.size > 100) {
                auto icontex = render::LoadJpegImage(img.memory, img.size);
                this->loadedicons.push_back(icontex);
            } else {
                this->loadedicons.push_back(render::LoadImage("romfs:/shrek.png"));
            }
            free(img.memory);
            /* rich name */
            auto rstrname = std::to_string(this->itms[i].id);
            auto rtex = render::RenderText(this->richfont, this->richfont_meme, rstrname, theme::text);
            this->loadedrichnames.push_back(rtex);
            /* rich icon */
            auto rstricon = nh::Lang::getFlag(itms[i].getLanguage());
            auto ricontex = render::LoadImage(rstricon);
            this->loadedrichicons.push_back(ricontex);
        }
    }
}