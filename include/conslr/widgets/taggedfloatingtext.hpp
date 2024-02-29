///
///@file conslr/widgets/taggedfloatingtext.hpp
///@brief Defines the TaggedFloatingText widget
///
#pragma once

#include <SDL_rect.h>

#include "conslr/widget.hpp"
#include "conslr/screen.hpp"
#include "conslr/taggedstring.hpp"

namespace conslr::widgets
{
    ///
    ///Renders borderless tagged text
    ///
    class TaggedFloatingText : public IWidget, public IRenderable
    {
    public:
        friend class conslr::Screen;
        friend class conslr::WidgetManager;

        //Getters
        constexpr const SDL_Rect& getRegion() const noexcept { return mRegion; }
        constexpr const TaggedString& getString() const noexcept { return mString; }
        constexpr const TagSet& getTags() const noexcept { return mTags; }

        //Setters
        constexpr void setRegion(const SDL_Rect& region) noexcept
        {
            assert((region.w > 0 && region.h > 0) && "Region is too small");

            mRegion = region;
            mRerender = true;

            return;
        }
        constexpr void setString(const TaggedString& str) { mString = str; mRerender = true; }
        constexpr void setTags(const TagSet& tags) { mTags = tags; mRerender = true; }

    protected:
        constexpr TaggedFloatingText(int32_t id, int32_t priority) noexcept :
            IWidget{ id, priority },
            mRegion{ 0, 0, 0, 0 }
        {}

        virtual void render(Screen& screen)
        {
            assert((mRegion.w > 0 && mRegion.h > 0) && "Region is too small to render");

            screen.renderMultilineTextTagged(
                    mRegion.x, mRegion.y,
                    mRegion.w, mRegion.h,
                    mString,
                    mTags
                    );

            return;
        }

        SDL_Rect mRegion;
        TaggedString mString;
        TagSet mTags;
    };
}
