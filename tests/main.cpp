#include "conslr/console.hpp"
#include "conslr/screen.hpp"
#include "conslr/widget.hpp"

#include <SDL.h>

class RectWidget : public conslr::IWidget, public conslr::IRenderable
{
public:
    RectWidget(int32_t index, int32_t priority) : IWidget{ index, priority } {}
    virtual void render(conslr::Screen& scr) override
    {
        scr.fillRectBackground(r, color);
    }

    SDL_Rect r;
    SDL_Color color;
};

int main(int argc, char* argv[])
{
    conslr::Console console(32, 64, 80, 24);
    if (console.init("TestConsole") < 0)
    {
        exit(-1);
    }

    int32_t font = console.createFont("ibm_vga_fontsheet.bmp", 8, 16);
    console.setCurrentFontIndex(font);

    int32_t scrIndex = console.createScreen();
    console.setCurrentScreenIndex(scrIndex);

    auto& wm = console.getWidgetManager(scrIndex);
    auto rect1 = wm.createWidget<RectWidget>(1);
    rect1.lock()->r = { 1, 1, 2, 2 };
    rect1.lock()->color = { 255, 0, 0, 255 };

    auto r2 = wm.createWidget<RectWidget>(0);
    r2.lock()->r = { 2, 2, 2, 2 };
    r2.lock()->color = { 0, 0, 255, 255 };

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
