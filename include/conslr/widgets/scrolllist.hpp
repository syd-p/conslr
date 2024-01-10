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

namespace conslr
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
        ///Used internal, should not be called directly
        ///
        ScrollList(int32_t id, int32_t priority) :
            IWidget{ id, priority },
            mRegion{ 0, 0, 0, 0 }, mScrollY{ 0 }
        {}

        virtual void render(Screen& screen) override
        {
            screen.borderRect(mRegion, '=', '|', '+');

            if (mElements.size() == 0)
            {
                return;
            }

            //Render the elements
            int32_t freeWidth = mRegion.w - 2;
            int32_t freeHeight = mRegion.h - 2;

            int32_t maxShown = std::min(freeHeight, (int32_t)mElements.size());
            for (auto i = 0; i < maxShown; i++)
            {
                const auto& element = mElements.at(mScrollY + i);
                screen.renderText(1, 1 + i, freeWidth, element.mName);
            }

            screen.setCellBackground(1, mSelection - mScrollY + 1, { 100, 100, 100, 255 });

            return;
        }

        void addElement(const T& t, const std::string& name)
        {
            mElements.emplace_back(mElements.size(), t, name);
            mRerender = true;

            return;
        }

        void addElement(T&& t, const std::string& name)
        {
            mElements.emplace_back(mElements.size(), t, name);
            mRerender = true;

            return;
        }

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

        const T& getCurrentElement() { return mElements.at(mSelection).mElement; }

        void setRegion(SDL_Rect region)
        {
            assert(((region.w > 2) && (region.h > 2)) && "Region is too small");

            mRegion = region;
            mRerender = true;
        }

    protected:
        std::vector<ScrollListContainer<T>> mElements;

        SDL_Rect mRegion;
        int32_t mScrollY;
    };
}
