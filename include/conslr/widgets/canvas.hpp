///
///@file conslr/widgets/canvas.hpp
///@brief Defines the Canvas widget
///
#pragma once

#include <functional>

#include <SDL_rect.h>

#include "conslr/widget.hpp"
#include "conslr/screen.hpp"
#include "conslr/theme.hpp"

namespace conslr::widgets
{
    ///
    ///Renders from a custom function
    ///
    class Canvas : public IWidget, public IRenderable
    {
    public:
        friend class conslr::Screen;
        friend class conslr::WidgetManager;

        constexpr const SDL_Rect& getRegion() const noexcept { return mRegion; }

        constexpr void setRegion(const SDL_Rect& region) noexcept { mRegion = region; }
        void setRenderFunction(std::function<void(const SDL_Rect&, conslr::Screen&)> func) { mRender = func; }

    protected:
        Canvas(int32_t id, int32_t priority) noexcept :
            IWidget{ id, priority },
            mRegion{ 0, 0, 0, 0 },
            mRender{ nullptr }
        {}

        virtual void render(conslr::Screen& screen) override
        { 
            assert((mRegion.w > 2 && mRegion.h > 2) && "Canvas region is too small");

            screen.borderRect(mRegion, 
                    mTheme->borderHorizontal, mTheme->borderVertical,
                    mTheme->borderCornerTl, mTheme->borderCornerTr,
                    mTheme->borderCornerBl, mTheme->borderCornerBr);

            if (mRender != nullptr)
            {
                SDL_Rect renderRegion{ mRegion.x + 1, mRegion.y + 1, mRegion.w - 2, mRegion.h - 2 };
                mRender(renderRegion, screen);
            }

            return;
        }

        SDL_Rect mRegion;
        std::function<void(const SDL_Rect&, conslr::Screen&)> mRender;
    };
}
