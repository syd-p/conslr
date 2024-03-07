///
///@file conslr/widgets/checklist.hpp
///@brief Defines the CheckList widget
///
#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <sstream>
#include <algorithm>

#include <SDL.h>

#include "conslr/widget.hpp"
#include "conslr/screen.hpp"
#include "conslr/theme.hpp"
#include "conslr/widgets/scrolllist.hpp"
#include "conslr/widgetmanager.hpp"
#include "conslr/widgetfactory.hpp"

namespace conslr::widgets
{
    ///
    ///CheckList widget
    ///Note that it returns elements as a pair (element, currently ticked)
    ///
    template<typename T>
    class CheckList : public IWidget, public IRenderable, public IScrollable
    {
    public:
        friend class conslr::Screen;
        friend class conslr::WidgetManager;

        void addElement(const T& t, const std::string& name, bool ticked = false)
        {
            mElements.emplace_back(std::pair<ScrollListContainer<T>, bool>{ { mElements.size(), t, name }, ticked });
            mRerender = true;

            return;
        }

        void addElement(T&& t, const std::string& name, bool ticked = false)
        {
            mElements.emplace_back(std::pair<ScrollListContainer<T>, bool>{ { mElements.size(), t, name }, ticked });
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

        constexpr void toggleCurrentElement() noexcept
        {
            mElements.at(mSelection).second = !mElements.at(mSelection).second;
            
            mRerender = true;

            return;
        }

        constexpr void showScrollbar() noexcept { mShowScrollbar = true; mRerender = true; }
        constexpr void hideScrollbar() noexcept { mShowScrollbar = false; mRerender = true; }

        //Getters
        constexpr const std::pair<ScrollListContainer<T>, bool> getCurrentElement() { return mElements.at(mSelection); }
        constexpr const SDL_Rect& getRegion() const noexcept { return mRegion; }
        constexpr const std::vector<std::pair<ScrollListContainer<T>, bool>>& getElements() const noexcept { return mElements; }

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
        constexpr void setElements(bool value)
        {
            for (auto& element : mElements)
            {
                element.second = value;
            }

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
                std::string checkbox = "[ ]";
                if (element.second)
                {
                    checkbox.at(1) = mTheme->selectionTick;
                }
                screen.renderTextColor(xOffset, yOffset + i, freeWidth, checkbox + element.first.mName, mTheme->text);
            }

            //Current selection
            if (mActive)
            {
                screen.setCellBackground(xOffset + 1, yOffset + mSelection - mScrollY, mTheme->selection);
            }

            if (mShowTitle)
            {
                screen.renderTextColor(
                        mRegion.x + 1, mRegion.y,
                        std::min(freeWidth, (int32_t)mTitle.size()),
                        mTitle,
                        mTheme->border);
            }

            if (mShowScrollbar && mElements.size() > (size_t)freeHeight)
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

        std::vector<std::pair<ScrollListContainer<T>, bool>> mElements;

        SDL_Rect mRegion;
        int32_t mScrollY;
        bool mShowScrollbar;
    };

    ///
    ///Populates a list if possible
    ///Custom data types must have their own specialized function
    ///
    template <typename T>
    inline void constructListElements(std::shared_ptr<CheckList<T>> ptr, const std::string& str)
    {
        (void)ptr; (void)str;
        assert(0 && "Unimplemented type for constructListElements");

        return;
    }

    //Signed int specialization
    template<std::signed_integral T>
    inline void constructListElements(std::shared_ptr<CheckList<T>> ptr, const std::string& str)
    {
        std::stringstream ss{ str };

        std::string name; 
        std::string val;
        bool ticked;

        while (ss >> name >> val >> ticked)
        {
            ptr->addElement(std::stoll(val), name, ticked);
            
        }

        return;
    }

    //Unsigned int specialization
    template<std::unsigned_integral T>
    inline void constructListElements(std::shared_ptr<CheckList<T>> ptr, const std::string& str)
    {
        std::stringstream ss{ str };

        std::string name; 
        std::string val;
        bool ticked;

        while (ss >> name >> val >> ticked)
        {
            ptr->addElement(std::stoull(val), name, ticked);
        }

        return;
    }

    //Floating point specialization
    template<std::floating_point T>
    inline void constructListElements(std::shared_ptr<CheckList<T>> ptr, const std::string& str)
    {
        std::stringstream ss{ str };

        std::string name; 
        std::string val;
        bool ticked;

        while (ss >> name >> val >> ticked)
        {
            ptr->addElement(std::stold(val), name, ticked);
        }

        return;
    }

    //String specialization
    template<>
    inline void constructListElements<std::string>(std::shared_ptr<CheckList<std::string>> ptr, const std::string& str)
    {
        std::stringstream ss{ str };

        std::string name; 
        std::string val;
        bool ticked;

        while (ss >> name >> val >> ticked)
        {
            ptr->addElement(val, name, ticked);
        }

        return;
    }

    //Char specialization
    template<>
    inline void constructListElements<char>(std::shared_ptr<CheckList<char>> ptr, const std::string& str)
    {
        std::stringstream ss{ str };

        std::string name; 
        char val;
        bool ticked;

        while (ss >> name >> val >> ticked)
        {
            ptr->addElement(val, name, ticked);
        }

        return;
    }

    ///
    ///Must be registered manually for all types used
    ///
    template <typename T>
    inline std::pair<std::string, int32_t> constructCheckList(WidgetManager& wm, const WidgetParameterMap& params)
    {
        int priority = 0;
        if (params.contains("priority"))
        {
            priority = std::stoi(params.at("priority"));
        }
        auto wptr = wm.createWidget<CheckList<T>>(priority);
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
