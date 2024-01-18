///
///@file conslr/widgets/taggedtextbox.hpp
///@brief
///
#pragma once

#include <SDL_rect.h>

#include "conslr/widget.hpp"
#include "conslr/screen.hpp"
#include "conslr/theme.hpp"
#include "conslr/taggedstring.hpp"

namespace conslr::widgets
{
    ///
    ///Renders a simple tagged text box
    ///
    class TaggedTextBox : public IWidget, public IRenderable
    {
    public:
        ///
        ///Internal constructor
        ///
        TaggedTextBox(int32_t id, int32_t priority) :
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

            screen.renderMultilineTextTagged(
                    mRegion.x + 1, mRegion.y + 1,
                    mRegion.w - 2, mRegion.h - 2,
                    mText,
                    mTheme->text, mTheme->background);

            return;
        }

        ///Returns the tagged string
        ///
        ///@return Reference to the tagged string
        TaggedString& getString() { return mText; }

        ///Sets the region of the widget
        ///
        ///@param region Region of the widget on screen
        void setRegion(SDL_Rect region)
        {
            assert(((region.w > 2) && (region.h > 2)) && "Region is too small");

            mRegion = region;
            mRerender = true;

            return;
        }

    protected:
        SDL_Rect mRegion; //!<Region of the widget on the screen
        TaggedString mText; //!<Text of the widget
    };
}
