///Example showing the creation and use of multiple screens
///
///Press 1 or 2 to change screens
#include <SDL.h>

#include "conslr/console.hpp"
#include "conslr/widgets/textbox.hpp"

int main()
{
    //Init console
    conslr::Console console{ 16, 32, 80, 24 };
    console.setTitle("Screens");

    //Create and set font for console
    int32_t font = console.createFont("res/ibm_vga_fontsheet.bmp", 8, 16);
    console.setCurrentFontIndex(font);

    //Create screen 1
    int32_t scr1 = console.createScreen();
    auto& wm1 = console.getWidgetManager(scr1);
    auto tb1 = wm1.createWidget<conslr::widgets::TextBox>();
    {
        auto tbPtr = tb1.lock();

        tbPtr->setRegion({ 0, 0, 40, 20 });
        tbPtr->setString("This is on screen 1!");
    }

    //Create screen 2
    int32_t scr2 = console.createScreen();
    auto& wm2 = console.getWidgetManager(scr2);
    auto tb2 = wm2.createWidget<conslr::widgets::TextBox>();
    {
        auto tbPtr = tb2.lock();

        tbPtr->setRegion({ 0, 0, 40, 20 });
        tbPtr->setString("This is on screen 2!");
    }

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
                //Changes the screen
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

        console.render();
    }

    SDL_Quit();

    return 0;
}
