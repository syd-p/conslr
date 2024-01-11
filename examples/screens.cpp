#include <SDL.h>

#include "conslr/console.hpp"
#include "conslr/widgets/textbox.hpp"

int main()
{
    conslr::Console console{ 32, 64, 80, 24 };
    if (console.init("Minimal Example") < 0)
    {
        exit(-1);
    }

    int32_t font = console.createFont("ibm_vga_fontsheet.bmp", 8, 16);
    console.setCurrentFontIndex(font);

    int32_t scr1 = console.createScreen();
    auto& wm1 = console.getWidgetManager(scr1);
    auto tb1 = wm1.createWidget<conslr::widgets::TextBox>();
    tb1.lock()->setRegion({ 0, 0, 40, 20 });
    tb1.lock()->setText("This is on screen 1!");

    int32_t scr2 = console.createScreen();
    auto& wm2 = console.getWidgetManager(scr2);
    auto tb2 = wm2.createWidget<conslr::widgets::TextBox>();
    tb2.lock()->setRegion({ 0, 0, 40, 20 });
    tb2.lock()->setText("This is on screen 2!");

    console.setCurrentScreenIndex(scr1);

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

            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.scancode == SDL_SCANCODE_1)
                {
                    console.setCurrentScreenIndex(scr1);
                }

                if (event.key.keysym.scancode == SDL_SCANCODE_2)
                {
                    console.setCurrentScreenIndex(scr2);
                }
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
