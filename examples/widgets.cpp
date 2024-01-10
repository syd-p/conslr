#include <string>
#include <iostream>

#include <SDL.h>

#include "conslr/console.hpp"
#include "conslr/widgets/scrolllist.hpp"

int main()
{
    conslr::Console console{ 32, 64, 80, 24 };
    if (console.init("Widget Example") < 0)
    {
        exit(-1);
    }

    int32_t font = console.createFont("ibm_vga_fontsheet.bmp", 8, 16);
    console.setCurrentFontIndex(font);

    int32_t scr = console.createScreen();
    console.setCurrentScreenIndex(scr);

    auto& wm = console.getWidgetManager(scr);
    auto listPtr = wm.createWidget<conslr::widgets::ScrollList<int32_t>>();
    listPtr.lock()->setRegion({ 1, 1, 20, 23 });
    listPtr.lock()->showTitle();
    listPtr.lock()->setTitle("List");
    for (auto i = 0; i < 30; i++)
    {
        listPtr.lock()->addElement(i, std::to_string(i));
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

            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.scancode == SDL_SCANCODE_UP)
                {
                    listPtr.lock()->scrollUp();
                }

                if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
                {
                    listPtr.lock()->scrollDown();
                }

                if (event.key.keysym.scancode == SDL_SCANCODE_RETURN)
                {
                    std::cout << "Currently selected list element is: " << listPtr.lock()->getCurrentElement() << std::endl;
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
