///
///@file conslr/widgets/floatingtext.hpp
///@brief Defines the FloatingText widget
///
#pragma once

#include <string>

#include <SDL_rect.h>

#include "conslr/widget.hpp"
#include "conslr/screen.hpp"
#include "conslr/theme.hpp"
#include "conslr/widgetmanager.hpp"
#include "conslr/widgetfactory.hpp"

namespace conslr::widgets
{
    ///
    ///Renders borderless text
    ///
    class FloatingText : public IWidget, public IRenderable
    {
    public:
        friend class conslr::Screen;
        friend class conslr::WidgetManager;

        //Getters
        constexpr const SDL_Rect& getRegion() const noexcept { return mRegion; }
        constexpr const std::string& getString() const noexcept { return mString; }

        constexpr void setRegion(const SDL_Rect& region) noexcept
        {
            assert((region.w > 0 && region.h > 0) && "Region is too small");

            mRegion = region;
            mRerender = true;

            return;
        }
        constexpr void setString(const std::string& str) { mString = str; mRerender = true; }

    protected:
        constexpr FloatingText(int32_t id, int32_t priority) noexcept :
            IWidget{ id, priority },
            mRegion{ 0, 0, 0, 0 }
        {}

        virtual void render(Screen& screen)
        {
            assert((mRegion.w > 0 && mRegion.h > 0) && "Region is too small to render");

            screen.renderMultilineTextColor(
                    mRegion.x, mRegion.y,
                    mRegion.w, mRegion.h,
                    mString,
                    mTheme->text, mTheme->background
                    );

            return;
        }

        SDL_Rect mRegion;
        std::string mString; //!<Text of the widget
    };

    inline void constructFloatingText(WidgetManager& wm, const WidgetParameterMap& params)
    {
        int priority = 0;
        if (params.contains("priority"))
        {
            priority = std::stoi(params.at("priority"));
        }
        auto wptr = wm.createWidget<FloatingText>(priority);

        wptr.lock()->setRegion({ 0, 0, 10, 2 });
        wptr.lock()->setString("Text");

        return;
    } 

}
