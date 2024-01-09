#include "conslr/console.hpp"
#include "conslr/screen.hpp"
#include "conslr/widget.hpp"

#include <SDL.h>

class TestWidget : public conslr::IWidget, public conslr::IRenderable
{
public:
    void render(conslr::Screen& scr) override
    {
        scr.fill({ 255, 0, 0, 255 }, { 0, 0, 255, 255 }, 'A');
    }
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
    wm.createWidget<TestWidget>();

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
