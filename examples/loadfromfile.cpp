#include <string>
#include <iostream>

#include <SDL.h>

#include "conslr/console.hpp"
#include "conslr/taggedstring.hpp"
#include "conslr/theme.hpp"
#include "conslr/themes/defaulttheme.hpp"
#include "conslr/widgets/scrolllist.hpp"
#include "conslr/widgets/textbox.hpp"
#include "conslr/widgets/taggedtextbox.hpp"
#include "conslr/widgets/floatingtext.hpp"

int main()
{
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);

    int cellW = 16;
    int cellH = 32;

    if (dm.w > 1920)
    {
        cellW = 32;
        cellH = 64;
    }

    //Init console
    conslr::Console console{ cellW, cellH, 80, 24 };
    console.setTitle("Widgets");

    //Init font
    int32_t font = console.createFont("ibm_vga_fontsheet.bmp", 8, 16);
    console.setCurrentFontIndex(font);

    //Init screen
    int32_t scr = console.createScreen();
    console.setCurrentScreenIndex(scr);

    console.getWidgetManager(scr).loadFromFile("str");

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

        console.render();
    }

    console.destroy();
    SDL_Quit();

    return 0;
}
