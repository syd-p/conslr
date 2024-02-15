///
///@file conslr/widgets/floatingtext.hpp
///@brief Defines the FloatingText widget
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
    ///Renders borderless text
    ///
    class FloatingText : public IWidget, public IRenderable
    {
    public:
        ///
        ///Internal constructor
        ///
        constexpr FloatingText(int32_t id, int32_t priority) noexcept :
            IWidget{ id, priority },
            mRegion{ 0, 0, 0, 0 }
        {}

        ///
        ///Internal
        ///
        virtual void render(Screen& screen)
        {
            assert((mRegion.w > 0 && mRegion.h > 0) && "Region is too small to render");

            screen.renderMultilineTextColor(
                    mRegion.x, mRegion.y,
                    mRegion.w, mRegion.h,
                    mText,
                    mTheme->text, mTheme->background
                    );

            return;
        }

        ///Sets the region of the widget
        ///
        ///@param region Region of the widget on screen
        constexpr void setRegion(const SDL_Rect& region) noexcept
        {
            assert((region.w > 0 && region.h > 0) && "Region is too small");

            mRegion = region;
            mRerender = true;

            return;
        }
        ///Sets the text of the widget
        ///
        ///@param text Text of the widget
        constexpr void setText(const std::string& text) 
        {
            mText = text;
            mRerender = true;

            return;
        }

    protected:
        SDL_Rect mRegion; //!<Region of the widget on the screen
        std::string mText; //!<Text of the widget
    };
}
