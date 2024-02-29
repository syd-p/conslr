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
        ///Returns the region
        ///
        ///@return Reference to the region
        constexpr const SDL_Rect& getRegion() const noexcept { return mRegion; }
        ///Returns the tagged string
        ///
        ///@return Reference to the tagged string
        constexpr const TaggedString& getString() const noexcept { return mString; }
        ///Returns the tag set
        ///
        ///@return Reference to the tag set
        constexpr const TagSet& getTags() const noexcept { return mTags; }

        //Setters
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
        ///Sets the string of the widget
        ///
        ///@param str String to set it to
        constexpr void setString(const TaggedString& str) { mString = str; mRerender = true; }
        ///Sets the tag set of the widget
        ///
        ///@param tags Tags to set it to
        constexpr void setTags(const TagSet& tags) { mTags = tags; mRerender = true; }

    protected:
        ///
        ///Internal constructor
        ///
        constexpr TaggedFloatingText(int32_t id, int32_t priority) noexcept :
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
                    mString,
                    mTags
                    );

            return;
        }

        SDL_Rect mRegion; //!<Region of the widget on the screen
        TaggedString mString; //!<Text of the widget
        TagSet mTags; //!<Tags of the widget
    };
}
