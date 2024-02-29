///
///@file conslr/widgets/textbox.hpp
///@brief Defines the TextBox widget
///
#pragma once

#include <string>

#include <SDL_rect.h>

#include "conslr/widget.hpp"
#include "conslr/screen.hpp"
#include "conslr/theme.hpp"
#include "conslr/widgetmanager.hpp"

namespace conslr::widgets
{
    ///
    ///Renders a simple text box
    ///
    class TextBox : public IWidget, public IRenderable
    {
    public:
        friend class conslr::Screen;
        friend class conslr::WidgetManager;

        //Getters
        constexpr const SDL_Rect& getRegion() const noexcept { return mRegion; }
        constexpr const std::string& getString() const noexcept { return mString; }

        //Setters
        constexpr void setRegion(const SDL_Rect& region) noexcept
        {
            assert(((region.w > 2) && (region.h > 2)) && "Region is too small");

            mRegion = region;
            mRerender = true;

            return;
        }
        constexpr void setString(const std::string& str) { mString = str; mRerender = true; }

    protected:
        constexpr TextBox(int32_t id, int32_t priority) noexcept :
            IWidget{ id, priority },
            mRegion{ 0, 0, 0, 0 }
        {}

        virtual void render(Screen& screen) override
        {
            assert((mRegion.w > 2 && mRegion.h > 2) && "Scroll List is too small to render");

            screen.fillRect(mRegion, mTheme->background, mTheme->border, 0);
            screen.borderRect(mRegion, mTheme->borderHorizontal, mTheme->borderVertical, mTheme->borderCornerTl, mTheme->borderCornerTr, mTheme->borderCornerBl, mTheme->borderCornerBr);

            if (mShowTitle)
            {
                screen.renderTextColor(
                        mRegion.x + 1, mRegion.y,
                        std::min(mRegion.w - 2, (int32_t)mTitle.size()),
                        mTitle,
                        mTheme->border
                        );
            }

            screen.renderMultilineTextColor(
                    mRegion.x + 1, mRegion.y + 1,
                    mRegion.w - 2, mRegion.h - 2,
                    mString,
                    mTheme->text);

            return;
        }

        SDL_Rect mRegion;
        std::string mString;
    };
}
