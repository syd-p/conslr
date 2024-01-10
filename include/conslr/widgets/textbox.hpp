///
///@file conslr/widgets/textbox.hpp
///@brief
///
#pragma once

#include <string>

#include <SDL_rect.h>

#include "conslr/widget.hpp"
#include "conslr/screen.hpp"
#include "conslr/colorscheme.hpp"

namespace conslr
{
    ///
    ///Renders a simple text box
    ///
    class TextBox : public IWidget, public IRenderable
    {
    public:
        TextBox(int32_t id, int32_t priority) :
            IWidget{ id, priority },
            mRegion{ 0, 0, 0, 0 }
        {}

        ///
        ///Internal
        ///
        virtual void render(Screen& screen) override
        {
            screen.fillRect(mRegion, mColorScheme->background, mColorScheme->border, 0);
            screen.borderRect(mRegion, '=', '|', '+');

            screen.renderMultilineTextColor(
                    mRegion.x + 1, mRegion.y + 1,
                    mRegion.w - 2, mRegion.h - 2,
                    mText,
                    mColorScheme->text);

            return;
        }

        void setRegion(SDL_Rect region)
        {
            assert(((region.w > 2) && (region.h > 2)) && "Region is too small");

            mRegion = region;
            mRerender = true;

            return;
        }
        void setText(const std::string& text)
        {
            mText = text;
            mRerender = true;

            return;
        }

    protected:
        SDL_Rect mRegion;
        std::string mText;
    };
}
