#include <string>

#include <SDL.h>

#include "conslr/console.hpp"
#include "conslr/widgets/scrolllist.hpp"

int main()
{
    //Init console
    conslr::Console console{ 16, 32, 80, 24 };
    console.setTitle("Widgets");

    //Init font
    int32_t font = console.createFont("ibm_vga_fontsheet.bmp", 8, 16);
    console.setCurrentFontIndex(font);

    //Init screen
    int32_t scr = console.createScreen();
    console.setCurrentScreenIndex(scr);

    auto widgetIds = console.getWidgetManager(scr).loadFromFile("examplescr.json");
    auto list = console.getWidgetManager(scr).getWidget<conslr::widgets::ScrollList<int32_t>>(widgetIds.at("ScrollList1"));

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
                if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
                {
                    list.lock()->scrollDown();
                }

                if (event.key.keysym.scancode == SDL_SCANCODE_UP)
                {
                    list.lock()->scrollUp();
                }
            }

            console.doEvent(event);
        }

        console.render();
    }

    console.destroy();
    SDL_Quit();

    return 0;
}
