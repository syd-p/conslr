///
///@file conslr/widgets/checklist.hpp
///@brief Defines the CheckList widget
///
#pragma once

#include <SDL.h>

#include "conslr/widget.hpp"
#include "conslr/screen.hpp"
#include "conslr/theme.hpp"
#include "conslr/widgetmanager.hpp"
#include "conslr/widgetfactory.hpp"

namespace conslr::widgets
{
    template<typename T>
    using CheckListContainer = std::pair<T, bool>;

    template<typename T>
    class CheckList : public IWidget, public IRenderable, public IScrollable, public IList<CheckListContainer<T>>
    {
    public:
        friend class conslr::Screen;
        friend class conslr::WidgetManager;

        virtual void addElement(const CheckListContainer<T>& t, const std::string& name) override
        {
            IList<CheckListContainer<T>>::addElement(t, name);
            mRerender = true;

            return;
        }

        virtual void removeElement(size_t index) override
        {
            IList<CheckListContainer<T>>::removeElement(index);
            if ((size_t)mSelection == IList<CheckListContainer<T>>::mElements.size())
            {
                //If the removed element was the last, and it was selected, scroll up
                scrollUp();
                mScrollY = std::max(0, mScrollY - 1);
            }
            else if (mScrollY != 0)
            {
                mScrollY--;
            }
            mRerender = true;

            return;
        }

        virtual constexpr void scrollUp() noexcept override
        {
            if (IList<CheckListContainer<T>>::mElements.size() == 0)
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
        virtual constexpr void scrollDown() noexcept override
        {
            if (IList<CheckListContainer<T>>::mElements.size() == 0)
            {
                return;
            }

            mSelection = std::min((int32_t)IList<CheckListContainer<T>>::mElements.size() - 1, mSelection + 1);
            //Subtracted by 1 to get the last element
            //without it it would be past the last element
            if (mSelection > mScrollY + mRegion.h - 2 - 1)
            {
                mScrollY++;
            }

            mRerender = true;

            return;
        }

        bool toggleCurrentElement() 
        {
            auto& element = IList<CheckListContainer<T>>::mElements.at(mSelection);
            element.mElement.second = !element.mElement.second;

            mRerender = true;

            return element.mElement.second;
        }

        constexpr void showScrollbar() noexcept { mShowScrollbar = true; mRerender = true; }
        constexpr void hideScrollbar() noexcept { mShowScrollbar = false; mRerender = true; }

        //Getters
        constexpr const ListContainer<CheckListContainer<T>>& getCurrentElement() { return IList<CheckListContainer<T>>::mElements.at(mSelection); }
        constexpr const SDL_Rect& getRegion() const noexcept { return mRegion; }

        //Setters
        constexpr void setRegion(const SDL_Rect& region) 
        {
            if (region.w <= 2 || region.h <= 2)
            {
                throw std::invalid_argument("Region width and height must be greater than 2, width: " + std::to_string(region.w) + ", height: " + std::to_string(region.h));
            }

            mRegion = region;
            mRerender = true;

            return;
        }

    protected:
        constexpr CheckList(int32_t id, int32_t priority) noexcept :
            IWidget{ id, priority },
            mRegion{ 0, 0, 0, 0 }, mScrollY{ 0 }, mShowScrollbar{ true }
        {}

        virtual void render(Screen& screen) override
        {
            if (mRegion.w <= 2 || mRegion.h <= 2)
            {
                throw std::runtime_error("Region width and height must be greater than 2, width: " + std::to_string(mRegion.w) + ", height: " + std::to_string(mRegion.h));
            }

            screen.fillRect(mRegion, mTheme->background, mTheme->border, 0);
            screen.borderRect(mRegion, mTheme->borderHorizontal, mTheme->borderVertical, mTheme->borderCornerTl, mTheme->borderCornerTr, mTheme->borderCornerBl, mTheme->borderCornerBr);

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

            if (IList<CheckListContainer<T>>::mElements.size() == 0)
            {
                return;
            }

            //Render the elements

            int32_t maxShown = std::min(freeHeight, (int32_t)IList<CheckListContainer<T>>::mElements.size());
            for (auto i = 0; i < maxShown; i++)
            {
                const auto& element = IList<CheckListContainer<T>>::mElements.at(mScrollY + i);
                screen.renderTextColor(xOffset, yOffset + i, freeWidth, "[ ]" + element.mName, mTheme->text);

                if (element.mElement.second)
                {
                    screen.setCellCharacter(xOffset + 1, yOffset +i, mTheme->selectionTick);
                }
            }

            //Current selection
            if (mActive)
            {
                screen.setCellBackground(xOffset + 1, yOffset + mSelection - mScrollY, mTheme->selection);
            }

            if (mShowScrollbar && IList<CheckListContainer<T>>::mElements.size() > (size_t)freeHeight)
            {
                double visiblePercent = (double)freeHeight / (double)IList<CheckListContainer<T>>::mElements.size(); //Percent of elements shown
                double percentDown = (double)mScrollY / (double)IList<CheckListContainer<T>>::mElements.size(); //How far down the first element is

                //Render scrollbar
                int32_t scrollbarOffset = percentDown * freeHeight;
                int32_t scrollbarHeight = visiblePercent * freeHeight;

                screen.fillRectCharacter({ mRegion.x, yOffset + scrollbarOffset, 1, std::min(scrollbarHeight + 1, freeHeight - scrollbarOffset) }, mTheme->scrollbarCharacter);
            }

            return;
        }

        SDL_Rect mRegion;
        int32_t mScrollY;
        bool mShowScrollbar;
    };
}
