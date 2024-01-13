///Example showing two basic themes
///
///Press 1 or 2 to change themes
#include <string>

#include <SDL.h>

#include "conslr/console.hpp"
#include "conslr/theme.hpp"
#include "conslr/widgets/textbox.hpp"

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
    tb.lock()->setRegion({ 1, 1, 40, 20 });
    tb.lock()->setText("Hello\nColor Schemes");
    tb.lock()->setTitle("Text Box");
    tb.lock()->showTitle();

    //Theme 1
    conslr::Theme cs1;
    cs1.background = { 255, 0, 0, 255 };
    cs1.border = { 0, 0, 255, 255 };
    cs1.text = { 0, 0, 0, 255 };
    cs1.borderCorner = '.';

    //Theme 2
    conslr::Theme cs2;
    cs2.background = { 0, 255, 0, 255 };
    cs2.border = { 255, 255, 255, 255 };
    cs2.text = { 255, 0, 255, 255 };

    console.setTheme(cs1);

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
                    console.setTheme(cs1);
                }

                if (event.key.keysym.scancode == SDL_SCANCODE_2)
                {
                    console.setTheme(cs2);
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
