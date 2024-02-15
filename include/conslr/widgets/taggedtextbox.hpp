///
///@file conslr/widgets/taggedtextbox.hpp
///@brief Defines the TaggedTextBox widget
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
        constexpr TaggedTextBox(int32_t id, int32_t priority) noexcept :
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
                    mString,
                    mTags);

            return;
        }

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
        constexpr void setString(const TaggedString& str) { mString = str; }
        ///Sets the tag set of the widget
        ///
        ///@param tags Tags to set it to
        constexpr void setTags(const TagSet& tags) { mTags = tags; }

    protected:
        SDL_Rect mRegion; //!<Region of the widget on the screen
        TaggedString mString; //!<Text of the widget
        TagSet mTags; //!<Tags of the widget
    };
}
