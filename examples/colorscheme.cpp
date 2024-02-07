///Example showing two basic themes
///
///Press 1 or 2 to change themes
#include <string>

#include <SDL.h>

#include "conslr/console.hpp"
#include "conslr/widgets/textbox.hpp"
#include "conslr/themes/defaulttheme.hpp"
#include "conslr/themes/turboblue.hpp"

int main()
{
    //Init console
    conslr::Console console{ 32, 64, 80, 24 };
    if (console.init("Color Scheme Example") < 0)
    {
        exit(-1);
    }

    //Init font
    int32_t font = console.createFont("ibm_vga_fontsheet.bmp", 8, 16);
    console.setCurrentFontIndex(font);

    //Init screen
    int32_t scr = console.createScreen();
    console.setCurrentScreenIndex(scr);

    //Creates a simple textbox widget
    auto& wm = console.getWidgetManager(scr);
    auto tb = wm.createWidget<conslr::widgets::TextBox>();
    {
        auto tbPtr = tb.lock();

        tbPtr->setRegion({ 1, 1, 40, 20 });
        tbPtr->setText("Hello\nColor Schemes");
        tbPtr->setTitle("Text Box");
        tbPtr->showTitle();
    }

    console.setTheme(conslr::themes::Default);

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
                //Changes the theme
                if (event.key.keysym.scancode == SDL_SCANCODE_1)
                {
                    console.setTheme(conslr::themes::Default);
                }

                if (event.key.keysym.scancode == SDL_SCANCODE_2)
                {
                    console.setTheme(conslr::themes::TurboBlue);
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
