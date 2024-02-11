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

        conslr::TaggedString str1{ conslr::createTaggedString("This is a tagged string\n", 0, 1) };
        conslr::TaggedString str2{ conslr::createTaggedString("With foreground\n", 2, 1) };
        conslr::TaggedString str3{ conslr::createTaggedString("With background\n", 0, 3) };
        conslr::TaggedString str4{ conslr::createTaggedString("With both", 2, 3) };
        conslr::TagSet tags;
        tags.at(0) = conslr::themes::Default.text;
        tags.at(1) = conslr::themes::Default.background;
        tags.at(2) = { 255, 0, 0, 255 };
        tags.at(3) = { 0, 0, 255, 255 };

        conslr::TaggedString str = str1;
        str.insert(str.end(), str2.begin(), str2.end());
        str.insert(str.end(), str3.begin(), str3.end());
        str.insert(str.end(), str4.begin(), str4.end());

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
