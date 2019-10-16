#include "ui/plutonium/elm_DetailedMenu.hpp"

namespace pu::ui::elm
{
    RichMenuItem::RichMenuItem(String Name)
    {
        this->clr = { 10, 10, 10, 255 };
        this->name = Name;
    }

    void RichMenuItem::AddOnClick(std::function<void()> Callback, u64 Key)
    {
        this->cbs.push_back(Callback);
        this->cbipts.push_back(Key);
    }

    s32 RichMenuItem::GetCallbackCount()
    {
        return this->cbs.size();
    }

    std::function<void()> RichMenuItem::GetCallback(s32 Index)
    {
        if(this->cbs.empty()) return [&](){};
        return this->cbs[Index];
    }

    u64 RichMenuItem::GetCallbackKey(s32 Index)
    {
        return this->cbipts[Index];
    }

    void RichMenuItem::SetIcon(std::string Icon)
    {
        std::ifstream ifs(Icon);
        if(ifs.good())
        {
            this->icon = Icon;
        } else {
            this->richicon = "romfs:/shrek.png";
        }
        ifs.close();
    }

    void RichMenuItem::SetRichIcon(std::string Icon)
    {
        std::ifstream ifs(Icon);
        if(ifs.good())
        {
            this->richicon = Icon;
        } else {
            this->richicon = "romfs:/shrek.png";
        }
        ifs.close();
    }

    RichMenu::RichMenu(s32 X, s32 Y, s32 Width, Color OptionColor, s32 ItemSize, s32 ItemsToShow) : Element::Element()
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
        this->basefont = render::LoadDefaultFont((ItemSize/3));
        this->richfont = render::LoadDefaultFont(ItemSize/4);
    }

    s32 RichMenu::GetX()
    {
        return this->x;
    }

    void RichMenu::SetX(s32 X)
    {
        this->x = X;
    }

    s32 RichMenu::GetY()
    {
        return this->y;
    }

    void RichMenu::SetY(s32 Y)
    {
        this->y = Y;
    }

    s32 RichMenu::GetWidth()
    {
        return this->w;
    }

    void RichMenu::SetWidth(s32 Width)
    {
        this->w = Width;
    }

    s32 RichMenu::GetHeight()
    {
        return (this->isize * this->ishow);
    }

    s32 RichMenu::GetItemSize()
    {
        return this->isize;
    }

    void RichMenu::SetItemSize(s32 ItemSize)
    {
        this->isize = ItemSize;
    }

    s32 RichMenu::GetNumberOfItemsToShow()
    {
        return this->ishow;
    }

    void RichMenu::SetNumberOfItemsToShow(s32 ItemsToShow)
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

    void RichMenu::SetOnSelectionChanged(std::function<void()> Callback)
    {
        this->onselch = Callback;
    }

    void RichMenu::AddItem(RichMenuItem::Ref &qItem)
    {
        this->itms.push_back(qItem);
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

    RichMenuItem::Ref &RichMenu::GetSelectedItem()
    {
        return this->itms[this->isel];
    }

    s32 RichMenu::GetSelectedIndex()
    {
        return this->isel;
    }

    void RichMenu::SetSelectedIndex(s32 Index)
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
            s32 cx = X;
            s32 cy = Y;
            s32 cw = this->w;
            s32 ch = this->isize;
            s32 its = this->ishow;
            if(its > this->itms.size()) its = this->itms.size();
            if((its + this->fisel) > this->itms.size()) its = this->itms.size() - this->fisel;
            if(this->loadednames.empty()) ReloadItemRenders();
            for(s32 i = this->fisel; i < (its + this->fisel); i++)
            {
                s32 clrr = this->clr.R;
                s32 clrg = this->clr.G;
                s32 clrb = this->clr.B;
                s32 nr = clrr - 70;
                if(nr < 0) nr = 0;
                s32 ng = clrg - 70;
                if(ng < 0) ng = 0;
                s32 nb = clrb - 70;
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
                RichMenuItem::Ref itm = this->itms[i];
                s32 xh = render::GetTextureHeight(curname);
                s32 tx = (cx + 25);
                s32 ty;
                if(itm->richname.empty()) ty = ((ch - xh) / 2) + cy;
                else ty = (ch/3) - (xh/2) + cy;
                if(!itm->icon.empty())
                {
                    float factor = (float)render::GetTextureHeight(curicon)/(float)render::GetTextureWidth(curicon);
                    s32 icw = (this->isize - 10);
                    s32 ich = icw;
                    s32 icx = (cx + 25);
                    s32 icy = (cy + 5);
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
                    Drawer->RenderTexture(curicon, icx, icy, { -1, icw, ich, -1.0f });
                }
                if(!itm->richname.empty())
                {
                    s32 rxh = render::GetTextureHeight(currichname);
                    s32 rtx = tx;
                    s32 rty = cy + (ch/3)*2 + ((ch/3) - rxh)/2;
                    if(!itm->richicon.empty())
                    {
                        float rfactor = (float)render::GetTextureHeight(currichicon)/(float)render::GetTextureWidth(currichicon);
                        s32 ricw = (ch/3);
                        s32 rich = ricw;
                        s32 ricx = rtx;
                        s32 ricy = (cy + (ch/3)*2);
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
                        Drawer->RenderTexture(currichicon, ricx, ricy, { -1, ricw, rich, -1.0f });
                    }
                    Drawer->RenderTexture(currichname, rtx, rty);
                }
                Drawer->RenderTexture(curname, tx, ty);
                cy += ch;
            }
            if(this->ishow < this->itms.size())
            {
                s32 sccr = this->scb.R;
                s32 sccg = this->scb.G;
                s32 sccb = this->scb.B;
                s32 snr = sccr - 30;
                if(snr < 0) snr = 0;
                s32 sng = sccg - 30;
                if(sng < 0) sng = 0;
                s32 snb = sccb - 30;
                if(snb < 0) snb = 0;
                Color sclr(snr, sng, snb, this->scb.A);
                s32 scx = this->x + (this->w - 20);
                s32 scy = this->y;
                s32 scw = 20;
                s32 sch = (this->ishow * this->isize);
                Drawer->RenderRectangleFill(this->scb, scx, scy, scw, sch);
                s32 fch = ((this->ishow * sch) / this->itms.size());
                s32 fcy = scy + (this->fisel * (sch / this->itms.size()));
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
                s32 cx = this->x;
                s32 cy = this->y;
                s32 cw = this->w;
                s32 ch = this->isize;
                s32 its = this->ishow;
                if(its > this->itms.size()) its = this->itms.size();
                if((its + this->fisel) > this->itms.size()) its = this->itms.size() - this->fisel;
                for(s32 i = this->fisel; i < (this->fisel + its); i++)
                {
                    if(((cx + cw) > Pos.X) && (Pos.X > cx) && ((cy + ch) > Pos.Y) && (Pos.Y > cy))
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
                    else (this->itms[this->isel]->GetCallback(0))();
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
                                if(!this->itms.empty()) for(s32 i = 0; i < this->itms.size(); i++)
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
                                if(!this->itms.empty()) for(s32 i = 0; i < this->itms.size(); i++)
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
                    s32 ipc = this->itms[this->isel]->GetCallbackCount();
                    if(ipc > 0) for(s32 i = 0; i < ipc; i++)
                    {
                        if(Down & this->itms[this->isel]->GetCallbackKey(i))
                        {
                            if(this->icdown) this->icdown = false;
                            else (this->itms[this->isel]->GetCallback(i))();
                        }
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
        s32 its = this->ishow;
        if(its > this->itms.size()) its = this->itms.size();
        if((its + this->fisel) > this->itms.size()) its = this->itms.size() - this->fisel;
        for(s32 i = this->fisel; i < (its + this->fisel); i++)
        {
            auto strname = this->itms[i]->name;
            auto tex = render::RenderText(this->basefont, strname, this->itms[i]->clr);
            this->loadednames.push_back(tex);
            if(!this->itms[i]->icon.empty())
            {
                auto stricon = this->itms[i]->icon;
                auto icontex = render::LoadImage(stricon);
                this->loadedicons.push_back(icontex);
            }
            else this->loadedicons.push_back(NULL);
            if(!this->itms[i]->richname.empty())
            {
                auto rstrname = this->itms[i]->richname;
                auto rtex = render::RenderText(this->richfont, rstrname, this->itms[i]->clr);
                this->loadedrichnames.push_back(rtex);
                if(!this->itms[i]->richicon.empty())
                {
                    auto rstricon = this->itms[i]->richicon;
                    auto ricontex = render::LoadImage(rstricon);
                    this->loadedrichicons.push_back(ricontex);
                }
                else this->loadedrichicons.push_back(NULL);
            }
            else
            {
                this->loadedrichnames.push_back(NULL);
                this->loadedrichicons.push_back(NULL);
            }
        }
    }
}