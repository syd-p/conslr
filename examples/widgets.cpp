///Example showing widgets
///
///Press up or down to scroll through list, enter to output the current element
#include <string>

#include <SDL.h>

#include "conslr/console.hpp"
#include "conslr/taggedstring.hpp"
#include "conslr/theme.hpp"
#include "conslr/themes/defaulttheme.hpp"
#include "conslr/widgets/textbox.hpp"
#include "conslr/widgets/taggedtextbox.hpp"
#include "conslr/widgets/floatingtext.hpp"
#include "conslr/widgets/scrolllist.hpp"

int main()
{
    //Init console
    conslr::Console console{ 16, 32, 80, 24 };
    console.setTitle("Widgets");

    //Init font
    int32_t font = console.createFont("res/ibm_vga_fontsheet.bmp", 8, 16);
    console.setCurrentFontIndex(font);

    //Init screen
    int32_t scr = console.createScreen();
    console.setCurrentScreenIndex(scr);

    auto& wm = console.getWidgetManager(scr);
    //Creates list
    auto l = wm.createWidget<conslr::widgets::ScrollList<int32_t>>();
    {
        auto listPtr = l.lock();

        listPtr->setRegion({ 1, 1, 20, 23 });
        listPtr->showTitle();
        listPtr->setTitle("List");
        for (auto i = 0; i < 30; i++)
        {
            listPtr->addElement(i, std::to_string(i));
        }
        wm.activateWidget(listPtr->getId());
    }

    //Creates text box
    auto tb = wm.createWidget<conslr::widgets::TextBox>();
    {
        auto tbPtr = tb.lock();

        tbPtr->setRegion({ 21, 0, 20, 8 });
        tbPtr->setString("This is a textbox!");
        tbPtr->showTitle();
        tbPtr->setTitle("Text Box");
    }

    auto ttb = wm.createWidget<conslr::widgets::TaggedTextBox>();
    {
        auto ttbPtr = ttb.lock();

        ttbPtr->setString(conslr::TaggedString("Tagged Strings ", 0, 1) + conslr::TaggedString("add color!", 2, 1));
        ttbPtr->setTags({ conslr::themes::Default.text, conslr::themes::Default.background, { 0, 255, 255, 255 } });
        ttbPtr->setRegion({ 41, 0, 20, 15 });
        ttbPtr->showTitle();
        ttbPtr->setTitle("Tagged Text Box");
    }

    auto ft = wm.createWidget<conslr::widgets::FloatingText>();
    {
        auto ftPtr = ft.lock();
        ftPtr->setRegion({ 41, 15, 20, 1 });
        ftPtr->setString("Floating text widget");
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
                const auto& keys = console.getKeyMap();

                if (event.key.keysym == keys.scrollUp)
                {
                    l.lock()->scrollUp();
                }

                if (event.key.keysym == keys.scrollDown)
                {
                    l.lock()->scrollDown();
                }

                if (event.key.keysym == keys.enter)
                {
                    if (l.lock()->size() > 0)
                    {
                        l.lock()->removeElement(l.lock()->getCurrentElement().mIndex);                    
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
