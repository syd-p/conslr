///Example showing RadioLists
///
///Press up or down to scroll through list, enter to select current element
#include <memory>
#include <string>

#include <SDL.h>

#include "conslr/console.hpp"
#include "conslr/widgets/radiolist.hpp"

int main()
{
    //Init console
    conslr::Console console{ 32, 64, 80, 24 };
    if (console.init("Widget Example") < 0)
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
    //Creates list
    auto listPtr = wm.createWidget<conslr::widgets::RadioList<int32_t>>();
    {
        auto sPtr = listPtr.lock();
        sPtr->setRegion({ 1, 1, 20, 23 });
        sPtr->showTitle();
        sPtr->setTitle("Radio List");
        for (auto i = 0; i < 30; i++)
        {
            sPtr->addElement(i, std::to_string(i));
        }
        wm.activateWidget(sPtr->getId());
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
                    listPtr.lock()->chooseElement();
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
