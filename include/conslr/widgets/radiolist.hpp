///
///@file conslr/widgets/radiolist.hpp
///@brief Defines the RadioList widget
///
#pragma once

#include <string>
#include <cassert>
#include <algorithm>
#include <sstream>
#include <stdexcept>

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
    class RadioList : public IWidget, public IRenderable, public IScrollable, public IList<T>
    {
    public:
        friend class conslr::Screen;
        friend class conslr::WidgetManager;

        virtual void addElement(const T& t, const std::string& name) override
        {
            IList<T>::addElement(t, name);
            mRerender = true;

            return;
        }

        virtual void removeElement(size_t index) override
        {
            IList<T>::removeElement(index);
            if ((size_t)mSelection == IList<T>::mElements.size())
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
            if (IList<T>::mElements.size() == 0)
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
            if (IList<T>::mElements.size() == 0)
            {
                return;
            }

            mSelection = std::min((int32_t)IList<T>::mElements.size() - 1, mSelection + 1);
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
        constexpr const ListContainer<T>& getCurrentElement() { return IList<T>::mElements.at(mSelection); }
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
        constexpr RadioList(int32_t id, int32_t priority) noexcept :
            IWidget{ id, priority },
            mRegion{ 0, 0, 0, 0 }, mScrollY{ 0 }, mShowScrollbar{ true },
            mChosenElement{ -1 }
        {}

        virtual void render(Screen& screen) override
        {
            if (mRegion.w <= 2 || mRegion.h <= 2)
            {
                throw std::runtime_error("Region width and height must be greater than 2, width: " + std::to_string(mRegion.w) + ", height: " + std::to_string(mRegion.h));
            }

            assert((mRegion.w > 2 && mRegion.h > 2) && "Scroll List is too small to render");

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

            if (IList<T>::mElements.size() == 0)
            {
                return;
            }

            //Render the elements

            int32_t maxShown = std::min(freeHeight, (int32_t)IList<T>::mElements.size());
            for (auto i = 0; i < maxShown; i++)
            {
                const auto& element = IList<T>::mElements.at(mScrollY + i);
                screen.renderTextColor(xOffset, yOffset + i, freeWidth, "[ ]" + element.mName, mTheme->text);

                if (mChosenElement == mScrollY + i)
                {
                    screen.setCellCharacter(xOffset + 1, yOffset + i, mTheme->selectionTick);
                }
            }

            //Current selection
            if (mActive)
            {
                screen.setCellBackground(xOffset + 1, yOffset + mSelection - mScrollY, mTheme->selection);
            }

            if (mShowScrollbar && IList<T>::mElements.size() > (size_t)freeHeight)
            {
                double visiblePercent = (double)freeHeight / (double)IList<T>::mElements.size(); //Percent of elements shown
                double percentDown = (double)mScrollY / (double)IList<T>::mElements.size(); //How far down the first element is

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
        int32_t mChosenElement; //!<Element currently chosen in the list, negative numbers are no selection
    };

    ///
    ///Populates a list if possible
    ///Custom data types must have their own specialized function
    ///
    template <typename T>
    inline void constructListElements(std::shared_ptr<RadioList<T>> ptr, const std::string& str)
    {
        (void)ptr; (void)str;
        assert(0 && "Unimplemented type for constructListElements");

        return;
    }

    //Signed int specialization
    template<std::signed_integral T>
    inline void constructListElements(std::shared_ptr<RadioList<T>> ptr, const std::string& str)
    {
        std::stringstream ss{ str };

        std::string name; 
        std::string val;

        while (ss >> name >> val)
        {
            ptr->addElement(std::stoll(val), name);
        }

        return;
    }

    //Unsigned int specialization
    template<std::unsigned_integral T>
    inline void constructListElements(std::shared_ptr<RadioList<T>> ptr, const std::string& str)
    {
        std::stringstream ss{ str };

        std::string name; 
        std::string val;

        while (ss >> name >> val)
        {
            ptr->addElement(std::stoull(val), name);
        }

        return;
    }

    //Floating point specialization
    template<std::floating_point T>
    inline void constructListElements(std::shared_ptr<RadioList<T>> ptr, const std::string& str)
    {
        std::stringstream ss{ str };

        std::string name; 
        std::string val;

        while (ss >> name >> val)
        {
            ptr->addElement(std::stold(val), name);
        }

        return;
    }

    //String specialization
    template<>
    inline void constructListElements<std::string>(std::shared_ptr<RadioList<std::string>> ptr, const std::string& str)
    {
        std::stringstream ss{ str };

        std::string name; 
        std::string val;

        while (ss >> name >> val)
        {
            ptr->addElement(val, name);
        }

        return;
    }

    //Char specialization
    template<>
    inline void constructListElements<char>(std::shared_ptr<RadioList<char>> ptr, const std::string& str)
    {
        std::stringstream ss{ str };

        std::string name; 
        char val;

        while (ss >> name >> val)
        {
            ptr->addElement(val, name);
        }

        return;
    }

    ///
    ///Must be registered manually for all types used
    ///
    template <typename T>
    inline std::pair<std::string, int32_t> constructRadioList(WidgetManager& wm, const WidgetParameterMap& params)
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
            constructListElements<T>(ptr, params.at("elements"));
        }

        if (params.contains("name"))
        {
            return { params.at("name"), ptr->getId() };
        }
        else
        {
            return { "unnamed", ptr->getId() };
        }
    }
}
