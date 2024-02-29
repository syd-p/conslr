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
        constexpr void setString(const TaggedString& str) { mString = str; }
        constexpr void setTags(const TagSet& tags) { mTags = tags; }

    protected:
        constexpr TaggedTextBox(int32_t id, int32_t priority) noexcept :
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

            screen.renderMultilineTextTagged(
                    mRegion.x + 1, mRegion.y + 1,
                    mRegion.w - 2, mRegion.h - 2,
                    mString,
                    mTags);

            return;
        }

        SDL_Rect mRegion;
        TaggedString mString;
        TagSet mTags;
    };
}
