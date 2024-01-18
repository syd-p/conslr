///
///Example showing the use of the RenderTextColor functions
///
#include <string>

#include <SDL.h>

#include "conslr/console.hpp"
#include "conslr/screen.hpp"
#include "conslr/theme.hpp"
#include "conslr/widget.hpp"

class RenderColorWidget : public conslr::IWidget, public conslr::IRenderable
{
public:
    RenderColorWidget(int32_t id, int32_t priority) : conslr::IWidget{ id, priority } {}

    virtual void render(conslr::Screen& screen) override
    {
        screen.fillRect(mRegion, mTheme->background, mTheme->border, 0);
        screen.borderRect(mRegion, mTheme->borderHorizontal, mTheme->borderVertical, mTheme->borderCornerTl, mTheme->borderCornerTr, mTheme->borderCornerBl, mTheme->borderCornerBr);

        screen.renderMultilineTextColor(
                mRegion.x + 1, mRegion.y + 1,
                mRegion.w - 2, mRegion.h - 2,
                mText,
                mForeground, mBackground);

        return;
    }

    SDL_Color mForeground;
    SDL_Color mBackground;
    SDL_Rect mRegion;
    std::string mText;
};

int main()
{
    //Init console
    conslr::Console console{ 32, 64, 80, 24 };
    if (console.init("Color Scheme Example") < 0)
    {
        exit(-1);
    }

    //Init font
    int32_t font = console.createFont("ibm_vga_fontsheet.bmp", 8, 16);
    console.setCurrentFontIndex(font);

    //Init screen
    int32_t scr = console.createScreen();
    console.setCurrentScreenIndex(scr);

    auto& wm = console.getWidgetManager(scr);
    auto rcw = wm.createWidget<RenderColorWidget>();
    {
        auto rcwPtr = rcw.lock();

        rcwPtr->mText = "This is some\nText!";
        rcwPtr->mRegion = { 0, 0, 20, 20 };
        rcwPtr->mBackground = { 0, 0, 255, 255 };
        rcwPtr->mForeground = { 255, 255, 255, 255 };
    }


    SDL_Event event;
    bool running = true;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }

            console.doEvent(event);
        }

        console.update();
        console.render();
    }

    console.destroy();
    SDL_Quit();

    return 0;
}
