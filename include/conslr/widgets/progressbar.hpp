///
///@file conslr/widgets/progressbar.hpp
///@brief Defines the ProgressBar widget
///
#pragma once

#include <algorithm>
#include <stdexcept>
#include <string>
#include <sstream>

#include <SDL.h>

#include "conslr/widget.hpp"
#include "conslr/screen.hpp"
#include "conslr/theme.hpp"
#include "conslr/widgetmanager.hpp"
#include "conslr/widgetfactory.hpp"

namespace conslr::widgets
{
    ///
    ///Renders a progress bar
    ///
    class ProgressBar : public IWidget, public IRenderable
    {
    public:
        friend class conslr::Screen;
        friend class conslr::WidgetManager;

        constexpr void addValue(const int32_t& val) noexcept { mCurrentValue = std::clamp(mCurrentValue + val, 0, mMaxValue); mRerender = true; }
        constexpr void removeValue(const int32_t& val) noexcept { mCurrentValue = std::clamp(mCurrentValue - val, 0, mMaxValue); mRerender = true; }

        constexpr void showPercent() noexcept { mShowPercent = true; mRerender = true; }
        constexpr void hidePercent() noexcept { mShowPercent = false; mRerender = true; }

        //Getters
        constexpr const SDL_Rect& getRegion() const noexcept { return mRegion; }
        constexpr const int32_t& getCurrentValue() const noexcept { return mCurrentValue; }
        constexpr const int32_t& getMaxValue() const noexcept { return mMaxValue; }

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
        constexpr void setCurrentValue(const int32_t& val) noexcept { mCurrentValue = std::clamp(val, 0, mMaxValue); mRerender = true; }
        constexpr void setMaxValue(const int32_t& val) noexcept { mMaxValue = std::max(val, 1); mRerender = true; }

    protected:
        constexpr ProgressBar(int32_t id, int32_t priority) noexcept :
           IWidget{ id, priority },
           mRegion{ 0, 0, 0, 0 },
           mCurrentValue{ 0 }, mMaxValue{ 100 },
           mShowPercent{ true }
        {}

        virtual void render(Screen& screen) override
        {
            if (mRegion.w <= 2 || mRegion.h <= 2)
            {
                throw std::runtime_error("Region width and height must be greater than 2, width: " + std::to_string(mRegion.w) + ", height: " + std::to_string(mRegion.h));
            }

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

            double percent = (double)mCurrentValue / (double)mMaxValue;
            int32_t fill = (int32_t)((double)(mRegion.w - 2) * percent);

            screen.fillRectForeground(
                    { mRegion.x + 1, mRegion.y + 1, fill, mRegion.h - 2 },
                    mTheme->text,
                    mTheme->progress);

            //Render percent
            if (mShowPercent && mRegion.w - 2 >= 4)
            {
                std::string p{ "%" };
                p += std::to_string((int32_t)(percent * 100));
                
                int32_t percentX = std::max(0, (int32_t)(((mRegion.w - 2) / 2) - (p.size() / 2)));
                
                screen.renderTextColor(
                        mRegion.x + 1 + percentX, mRegion.y + 1 + ((mRegion.h - 2) / 2),
                        mRegion.w - 2,
                        p,
                        mTheme->text);
            }

            return;
        }

        SDL_Rect mRegion;
        int32_t mCurrentValue;
        int32_t mMaxValue;
        bool mShowPercent;
    };

    inline std::pair<std::string, int32_t> constructProgressBar(WidgetManager& wm, const WidgetParameterMap& params)
    {
        int priority = 0;
        if (params.contains("priority"))
        {
            priority = std::stoi(params.at("priority"));
        }
        auto wptr = wm.createWidget<ProgressBar>(priority);
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

        if (params.contains("maxvalue"))
        {
            ptr->setMaxValue(std::stoi(params.at("maxvalue")));
        }

        if (params.contains("value"))
        {
            ptr->setCurrentValue(std::stoi(params.at("value")));
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
