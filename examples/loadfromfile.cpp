#include <string>

#include <SDL.h>

#include "conslr/console.hpp"
#include "conslr/theme.hpp"
#include "conslr/widgets/checklist.hpp"

int main()
{
    //Init console
    conslr::Console console{ 16, 32, 80, 24 };
    console.setTitle("Widgets");

    //Init font
    int32_t font = console.createFont("res/ibm_vga_fontsheet.bmp", 8, 16);
    console.setCurrentFontIndex(font);

    //Init theme
    conslr::Theme theme = console.createTheme("res/themes/turboblue.txt");
    console.setTheme(theme);

    //Init screen
    int32_t scr = console.createScreen();
    console.setCurrentScreenIndex(scr);

    //Load widgets from a file
    auto& wm = console.getWidgetManager(scr);
    auto widgetIds = wm.loadWidgetsFromFile("res/screens/loadfromfilescr.json");
    auto list = wm.getWidget<conslr::widgets::CheckList<int32_t>>(widgetIds.at("CheckList1"));

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

                if (event.key.keysym.scancode == SDL_SCANCODE_RETURN)
                {
                    if (list.lock()->size() != 0)
                    {
                        list.lock()->toggleCurrentElement();
                    }
                }
            }

            console.doEvent(event);
        }

        console.render();
    }

    SDL_Quit();

    return 0;
}
