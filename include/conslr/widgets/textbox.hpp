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

namespace conslr::widgets
{
    ///
    ///Renders a simple text box
    ///
    class TextBox : public IWidget, public IRenderable
    {
    public:
        friend class Screen;

        ///
        ///Internal constructor
        ///
        constexpr TextBox(int32_t id, int32_t priority) noexcept :
            IWidget{ id, priority },
            mRegion{ 0, 0, 0, 0 }
        {}

        //Getters
        ///Gets the region
        ///
        ///@return Reference to the region
        constexpr const SDL_Rect& getRegion() const noexcept { return mRegion; }
        ///Gets the string
        ///
        ///@return Reference to the text
        constexpr const std::string& getString() const noexcept { return mString; }

        //Setters
        ///Sets the region of the widget
        ///
        ///@param region Region of the widget on screen
        constexpr void setRegion(const SDL_Rect& region) noexcept
        {
            assert(((region.w > 2) && (region.h > 2)) && "Region is too small");

            mRegion = region;
            mRerender = true;

            return;
        }
        ///Sets the text of the widget
        ///
        ///@param str Text of the widget
        constexpr void setString(const std::string& str) { mString = str; mRerender = true; }

    protected:
        ///
        ///Internal
        ///
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

        SDL_Rect mRegion; //!<Region of the widget on the screen
        std::string mString; //!<Text of the widget
    };
}
