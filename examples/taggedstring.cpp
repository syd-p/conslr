///
///Example showing the use of the RenderTextTagged functions
///
#include <SDL.h>

#include "conslr/console.hpp"
#include "conslr/taggedstring.hpp"
#include "conslr/themes/defaulttheme.hpp"
#include "conslr/widgets/taggedtextbox.hpp"

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
    console.setTitle("Tagged String");

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

        conslr::TaggedString str{
            conslr::TaggedString("This is a tagged string\n", 0, 1) +
            conslr::TaggedString("With foreground\n", 2, 1) +
            conslr::TaggedString("With background\n", 0, 3) +
            conslr::TaggedString("With both", 2, 3)
        };

        conslr::TagSet tags;
        tags.at(0) = conslr::themes::Default.text;
        tags.at(1) = conslr::themes::Default.background;
        tags.at(2) = { 255, 0, 0, 255 };
        tags.at(3) = { 0, 0, 255, 255 };

        ttbPtr->setString(str);
        ttbPtr->setTags(tags);
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

        console.render();
    }

    console.destroy();
    SDL_Quit();

    return 0;
}
