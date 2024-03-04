///
///@file conslr/widgets/radiolist.hpp
///@brief Defines the RadioList widget
///
#pragma once

#include <vector>
#include <string>
#include <cassert>
#include <algorithm>
#include <sstream>

#include <SDL_rect.h>

#include "conslr/widget.hpp"
#include "conslr/screen.hpp"
#include "conslr/theme.hpp"
#include "conslr/widgets/scrolllist.hpp"
#include "conslr/widgetmanager.hpp"
#include "conslr/widgetfactory.hpp"

namespace conslr::widgets
{
    ///Allows the user to create a radio list of items
    ///Internal note, mSelection refers to what is being hovered (taken from scrollable), mChosenElement refers to the element of the list that is ticked
    ///
    ///@tparam Type of the elements of the ScrollList
    template<typename T>
    class RadioList : public IWidget, public IRenderable, public IScrollable
    {
    public:
        friend class conslr::Screen;
        friend class conslr::WidgetManager;

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

        virtual constexpr void scrollUp() noexcept override
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
        virtual constexpr void scrollDown() noexcept override
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
        virtual constexpr void chooseElement() noexcept
        {
            mChosenElement = mSelection;

            mRerender = true;
            return;
        }

        constexpr void showScrollbar() noexcept { mShowScrollbar = true; mRerender = true; }
        constexpr void hideScrollbar() noexcept { mShowScrollbar = false; mRerender = true; }

        //Getters
        constexpr const T& getCurrentElement() { return mElements.at(mSelection).mElement; }
        constexpr const SDL_Rect& getRegion() const noexcept { return mRegion; }

        //Setters
        constexpr void setRegion(const SDL_Rect& region) noexcept
        {
            assert(((region.w > 2) && (region.h > 2)) && "Region is too small");

            mRegion = region;
            mRerender = true;

            return;
        }

    protected:
        constexpr RadioList(int32_t id, int32_t priority) noexcept :
            IWidget{ id, priority },
            mRegion{ 0, 0, 0, 0 }, mScrollY{ 0 }, mShowScrollbar{ true },
            mChosenElement{ -1 }
        {}

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

            int32_t maxShown = std::min(freeHeight, (int32_t)mElements.size());
            for (auto i = 0; i < maxShown; i++)
            {
                const auto& element = mElements.at(mScrollY + i);
                screen.renderTextColor(xOffset, yOffset + i, freeWidth, "[ ]" + element.mName, mTheme->text);

                if (mChosenElement == mScrollY + i)
                {
                    screen.setCellCharacter(xOffset + 1, yOffset + i, mTheme->selectionTick);
                }
            }

            //Current selection
            if (mActive)
            {
                screen.setCellBackground(xOffset, yOffset + mSelection - mScrollY, mTheme->selection);
            }

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
                double visiblePercent = (double)freeHeight / (double)mElements.size(); //Percent of elements shown
                double percentDown = (double)mScrollY / (double)mElements.size(); //How far down the first element is

                //Render scrollbar
                int32_t scrollbarOffset = percentDown * freeHeight;
                int32_t scrollbarHeight = visiblePercent * freeHeight;

                screen.fillRectCharacter({ mRegion.x, yOffset + scrollbarOffset, 1, std::min(scrollbarHeight + 1, freeHeight - scrollbarOffset) }, mTheme->scrollbarCharacter);
            }

            return;
        }

        std::vector<ScrollListContainer<T>> mElements;

        SDL_Rect mRegion;
        int32_t mScrollY;
        bool mShowScrollbar;
        int32_t mChosenElement; //!<Element currently chosen in the list, negative numbers are no selection
    };

    ///
    ///Must be registered manually for all types used
    ///
    template <typename T>
    inline void constructRadioList(WidgetManager& wm, const WidgetParameterMap& params)
    {
        int priority = 0;
        if (params.contains("priority"))
        {
            priority = std::stoi(params.at("priority"));
        }
        auto wptr = wm.createWidget<RadioList<T>>(priority);
        auto ptr = wptr.lock();

        if (params.contains("visible"))
        {
            if (params.at("visible") == "true")
            {
                ptr->show();
            } 
            else if (params.at("visible") == "false")
            {
                ptr->hide();
            }
            else
            {
                throw std::invalid_argument("Param visible must be \"true\" or \"false\"");
            }
        }

        if (params.contains("showtitle"))
        {
            if (params.at("showtitle") == "true")
            {
                ptr->showTitle();
            }
            else if (params.at("showtitle") == "false")
            {
                ptr->hideTitle();
            } 
            else
            {
                throw std::invalid_argument("Param showtitle must be \"true\" or \"false\"");
            }
        }

        if (params.contains("active"))
        {
            if (params.at("active") == "true")
            {
                ptr->setActive(true);
            }
            else if (params.at("active") == "false")
            {
                ptr->setActive(false);
            } 
            else
            {
                throw std::invalid_argument("Param active must be \"true\" or \"false\"");
            }
        }

        if (params.contains("string"))
        {
            ptr->setString(params.at("string"));
        }

        if (params.contains("title"))
        {
            ptr->setTitle(params.at("title"));
        }

        if (params.contains("region"))
        {
            std::stringstream ss{ params.at("region") };
            int32_t x;
            int32_t y;
            int32_t w;
            int32_t h;

            ss >> x >> y >> w >> h;
            if (ss.fail())
            {
                throw std::invalid_argument("Param region must be in the format of \"intx inty intw inth\"");
            }

            ptr->setRegion({ x, y, w, h });
        }

        if (params.contains("elements"))
        {
            //Todo load elements
        }

        return;
    } 
}
