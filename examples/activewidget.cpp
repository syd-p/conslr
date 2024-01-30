///Example showing active widgets
///
///Press up or down to scroll through list, enter to output the current element
///Press tab to change the active list
#include <memory>
#include <string>
#include <iostream>

#include <SDL.h>

#include "conslr/console.hpp"
#include "conslr/widgets/scrolllist.hpp"
#include "conslr/themes/turboblue.hpp"

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

    console.setTheme(conslr::themes::TurboBlue);

    auto& wm = console.getWidgetManager(scr);
    //Creates list
    auto listPtr = wm.createWidget<conslr::widgets::ScrollList<int32_t>>();
    {
        auto sPtr = listPtr.lock();
        sPtr->setRegion({ 1, 1, 20, 23 });
        sPtr->showTitle();
        sPtr->setTitle("List");
        for (auto i = 0; i < 30; i++)
        {
            sPtr->addElement(i, std::to_string(i));
        }
        wm.activateWidget(sPtr->getId());
    }

    auto listPtr2 = wm.createWidget<conslr::widgets::ScrollList<char>>();
    {
        auto sPtr = listPtr2.lock();
        sPtr->setRegion({ 21, 1, 20, 23 });
        sPtr->showTitle();
        sPtr->setTitle("List 2");
        for (char i = 'a'; i < 'z'; i++)
        {
            sPtr->addElement(i, std::string{} + i);
        }
    }

    int32_t currentWidget = 0;

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
                    if (currentWidget == 0)
                    {
                        listPtr.lock()->scrollUp();
                    } else
                    {
                        listPtr2.lock()->scrollUp();
                    }
                }

                if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
                {
                    if (currentWidget == 0)
                    {
                        listPtr.lock()->scrollDown();
                    } else
                    {
                        listPtr2.lock()->scrollDown();
                    }
                }

                if (event.key.keysym.scancode == SDL_SCANCODE_RETURN)
                {
                    if (currentWidget == 0)
                    {
                        std::cout << "Currently selected list element is: " << listPtr.lock()->getCurrentElement() << std::endl;
                    } else
                    {
                        std::cout << "Currently selected list element is: " << listPtr2.lock()->getCurrentElement() << std::endl;
                    }
                }

                if (event.key.keysym.scancode == SDL_SCANCODE_TAB)
                {
                    if (listPtr.lock()->getActive())
                    {
                        wm.activateWidget(listPtr2.lock()->getId());
                        wm.deactivateWidget(listPtr.lock()->getId());
                        currentWidget = 1;
                    } else
                    {
                        wm.activateWidget(listPtr.lock()->getId());
                        wm.deactivateWidget(listPtr2.lock()->getId());
                        currentWidget = 0;
                    }
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
