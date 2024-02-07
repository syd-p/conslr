///
///Example showing the use of the RenderTextTagged functions
///
#include <string>

#include <SDL.h>

#include "conslr/console.hpp"
#include "conslr/screen.hpp"
#include "conslr/taggedstring.hpp"
#include "conslr/widgets/taggedtextbox.hpp"

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

    auto& wm = console.getWidgetManager(scr);
    auto ttb = wm.createWidget<conslr::widgets::TaggedTextBox>();
    {
        auto ttbPtr = ttb.lock();

        //Tagged strings work like a stringstream, where FgTag and BgTag are used to set the index of the foreground and background tags respectively
        //The index 0 is for the default foreground/background of the widget which typically comes from the theme
        ttbPtr->getString() << "Some tagged text\n" << conslr::FgTag(1) << "With foreground\n" << conslr::FgTag(0) << conslr::BgTag(2) << "With background\n" << conslr::FgTag(1) << "With both!";
        ttbPtr->getString().setTag(1, { 0, 0, 255, 255 });
        ttbPtr->getString().setTag(2, { 255, 0, 0, 255 });
        ttbPtr->setRegion({ 0, 0, 20, 20 });
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

            console.doEvent(event);
        }

        console.update();
        console.render();
    }

    console.destroy();
    SDL_Quit();

    return 0;
}
