///Example showing widgets
///
///Press up or down to scroll through list, enter to output the current element
#include <string>
#include <iostream>

#include <SDL.h>

#include "conslr/console.hpp"
#include "conslr/taggedstring.hpp"
#include "conslr/widgets/scrolllist.hpp"
#include "conslr/widgets/textbox.hpp"
#include "conslr/widgets/taggedtextbox.hpp"
#include "conslr/widgets/floatingtext.hpp"

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
    auto listPtr = wm.createWidget<conslr::widgets::ScrollList<int32_t>>();
    listPtr.lock()->setRegion({ 1, 1, 20, 23 });
    listPtr.lock()->showTitle();
    listPtr.lock()->setTitle("List");
    for (auto i = 0; i < 30; i++)
    {
        listPtr.lock()->addElement(i, std::to_string(i));
    }
    wm.activateWidget(listPtr.lock()->getId());

    //Creates text box
    auto tbPtr = wm.createWidget<conslr::widgets::TextBox>();
    tbPtr.lock()->setRegion({ 21, 0, 20, 8 });
    tbPtr.lock()->setText("This is a textbox!");
    tbPtr.lock()->showTitle();
    tbPtr.lock()->setTitle("Text Box");

    auto ttb = wm.createWidget<conslr::widgets::TaggedTextBox>();
    {
        auto ttbPtr = ttb.lock();
        ttbPtr->setRegion({ 41, 0, 20, 15 });
        ttbPtr->showTitle();
        ttbPtr->setTitle("Tagged Text Box");

        using conslr::FgTag;
        using conslr::BgTag;
        auto& ts = ttbPtr->getString();
        ts << "This is a tagged string\n" << FgTag(1) << "It can have multiple colors\n" << BgTag(2) << "Using tags!";
        ts.setTag(1, { 255, 0, 0, 255 });
        ts.setTag(2, { 0, 0, 255, 255 });
    }

    auto ft = wm.createWidget<conslr::widgets::FloatingText>();
    {
        auto ftPtr = ft.lock();
        ftPtr->setRegion({ 41, 15, 20, 2 });
        ftPtr->setText("Floating text widget");
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
