///
///@file conslr/widgets/scrolllist.hpp
///@brief
///
#pragma once

#include <vector>
#include <string>
#include <cassert>
#include <algorithm>

#include <SDL_rect.h>

#include "conslr/widget.hpp"
#include "conslr/screen.hpp"
#include "conslr/theme.hpp"

namespace conslr::widgets
{
    ///Container used to store elements in a scroll list
    ///
    ///@tparam Type of element to hold
    template<typename T>
    struct ScrollListContainer
    {
        const int32_t mSelectionIndex; //!<The index of this element in the vector
        T mElement; //!<Stored element
        std::string mName; //!<String to be rendered for the selection
    };

    ///Allows the user to create a list of items
    ///
    ///@tparam Type of the elements of the ScrollList
    template<typename T>
    class ScrollList : public IWidget, public IRenderable, public IScrollable
    {
    public:
        ///
        ///Used internally, should not be called directly
        ///
        ScrollList(int32_t id, int32_t priority) :
            IWidget{ id, priority },
            mRegion{ 0, 0, 0, 0 }, mScrollY{ 0 }, mShowScrollbar{ true }
        {}

        ///
        ///Internal
        ///
        virtual void render(Screen& screen) override
        {
            assert((mRegion.w > 2 && mRegion.h > 2) && "Scroll List is too small to render");

            screen.fillRect(mRegion, mTheme->background, mTheme->border, 0);
            screen.borderRect(mRegion, mTheme->borderHorizontal, mTheme->borderVertical, mTheme->borderCornerTl, mTheme->borderCornerTr, mTheme->borderCornerBl, mTheme->borderCornerBr);

            if (mElements.size() == 0)
            {
                return;
            }

            //Render the elements
            int32_t xOffset = mRegion.x + 1;
            int32_t yOffset = mRegion.y + 1;
            int32_t freeWidth = mRegion.w - 2;
            int32_t freeHeight = mRegion.h - 2;

            if (mShowTitle)
            {
                screen.renderTextColor(
                        mRegion.x + 1, mRegion.y,
                        std::min(freeWidth, (int32_t)mTitle.size()),
                        mTitle,
                        mTheme->border);
            }

            if (mShowScrollbar && mElements.size() > freeHeight)
            {
                int32_t visiblePercent = (freeHeight * 100) / mElements.size(); //Percent of elements shown
                int32_t percentDown = (mScrollY * 100) / mElements.size(); //How far down the first element is

                //Render scrollbar
                int32_t scrollbarOffset = (percentDown * freeHeight) / 100;
                int32_t scrollbarHeight = (visiblePercent * freeHeight) / 100;

                screen.fillRectCharacter({ mRegion.x, yOffset + scrollbarOffset, 1, std::min(scrollbarHeight + 1, freeHeight - scrollbarOffset) }, mTheme->scrollbarCharacter);
            }

            int32_t maxShown = std::min(freeHeight, (int32_t)mElements.size());
            for (auto i = 0; i < maxShown; i++)
            {
                const auto& element = mElements.at(mScrollY + i);
                screen.renderTextColor(xOffset, yOffset + i, freeWidth, element.mName, mTheme->text);
            }

            //Current selection
            if (mActive)
            {
                screen.setCellBackground(xOffset, yOffset + mSelection - mScrollY, mTheme->selection);
            }

            return;
        }

        ///Adds an element to the list
        ///
        ///@param t Object value to be added
        ///@param name Name for rendering the list
        void addElement(const T& t, const std::string& name)
        {
            mElements.emplace_back(mElements.size(), t, name);
            mRerender = true;

            return;
        }
        ///Adds an element to the list
        ///
        ///@param t Object value to be added
        ///@param name Name for rendering the list
        void addElement(T&& t, const std::string& name)
        {
            mElements.emplace_back(mElements.size(), t, name);
            mRerender = true;

            return;
        }

        ///
        ///Scrolls the list up
        ///
        virtual void scrollUp() override
        {
            if (mElements.size() == 0)
            {
                return;
            }

            mSelection = std::max(0, mSelection - 1);
            if (mSelection < mScrollY)
            {
                mScrollY--;
            }

            mRerender = true;

            return;
        }
        ///
        ///Scrolls the list down
        ///
        virtual void scrollDown() override
        {
            if (mElements.size() == 0)
            {
                return;
            }

            mSelection = std::min((int32_t)mElements.size() - 1, mSelection + 1);
            //Subtracted by 1 to get the last element
            //without it it would be past the last element
            if (mSelection > mScrollY + mRegion.h - 2 - 1)
            {
                mScrollY++;
            }

            mRerender = true;

            return;
        }

        ///
        ///Shows the scrollbar if possible
        ///
        void showScrollbar() { mShowScrollbar = true; mRerender = true; }
        ///
        ///Hides the scrollbar
        ///
        void hideScrollbar() { mShowScrollbar = false; mRerender = true; }

        //Getters
        ///Gets the current elements value
        ///
        ///@return Value of the currently selected element
        const T& getCurrentElement() { return mElements.at(mSelection).mElement; }
        ///Gets the region of the widget
        ///
        ///@return Region
        const SDL_Rect& getRegion() const { return mRegion; }

        //Setters
        ///Sets the region of the widget
        ///
        ///@param region Region in cells of the widget
        void setRegion(const SDL_Rect& region)
        {
            assert(((region.w > 2) && (region.h > 2)) && "Region is too small");

            mRegion = region;
            mRerender = true;

            return;
        }

    protected:
        std::vector<ScrollListContainer<T>> mElements; //!<Elements of the list

        SDL_Rect mRegion; //!<Region of the widget on screen
        int32_t mScrollY; //!<Scroll offset of the list
        bool mShowScrollbar; //!<Shows scrollbar if true
    };
}
