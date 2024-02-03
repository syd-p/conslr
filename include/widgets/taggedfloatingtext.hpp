///
///@file conslr/widgets/taggedfloatingtext.hpp
///@brief Defines the TaggedFloatingText widget
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
    ///Renders borderless tagged text
    ///
    class TaggedFloatingText : public IWidget, public IRenderable
    {
    public:
        ///
        ///Internal constructor
        ///
        TaggedFloatingText(int32_t id, int32_t priority) :
            IWidget{ id, priority },
            mRegion{ 0, 0, 0, 0 }
        {}

        ///
        ///Internal
        ///
        virtual void render(Screen& screen)
        {
            assert((mRegion.w > 0 && mRegion.h > 0) && "Region is too small to render");

            screen.renderMultilineTextTagged(
                    mRegion.x, mRegion.y,
                    mRegion.w, mRegion.h,
                    mText,
                    mTheme->text, mTheme->background
                    );

            return;
        }

        ///Returns the tagged string
        ///
        ///@return Reference to the tagged string
        TaggedString& getString() { return mText; }

        ///Sets the region of the widget
        ///
        ///@param region Region of the widget on screen
        void setRegion(const SDL_Rect& region)
        {
            assert((region.w > 0 && region.h > 0) && "Region is too small");

            mRegion = region;
            mRerender = true;

            return;
        } 

    protected:
        SDL_Rect mRegion; //!<Region of the widget on the screen
        TaggedString mText; //!<Text of the widget
    };
}
