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
        ///
        ///Internal constructor
        ///
        TextBox(int32_t id, int32_t priority) :
            IWidget{ id, priority },
            mRegion{ 0, 0, 0, 0 }
        {}

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
                    mText,
                    mTheme->text);

            return;
        }

        ///Sets the region of the widget
        ///
        ///@param region Region of the widget on screen
        void setRegion(const SDL_Rect& region)
        {
            assert(((region.w > 2) && (region.h > 2)) && "Region is too small");

            mRegion = region;
            mRerender = true;

            return;
        }
        ///Sets the text of the widget
        ///
        ///@param text Text of the widget
        void setText(const std::string& text)
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
