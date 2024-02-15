///Example showing text input
///
///Enter text into the text box, press left ctrl to print the text
#include <string>
#include <iostream>
#include <chrono>

#include <SDL.h>

#include "conslr/console.hpp"
#include "conslr/keymapping.hpp"
#include "conslr/widgets/textbox.hpp"
#include "conslr/widgets/textinput.hpp"

int main()
{
    //Init console
    conslr::Console console{ 32, 64, 80, 24 };
    console.setTitle("Text Input");

    //Init font
    int32_t font = console.createFont("ibm_vga_fontsheet.bmp", 8, 16);
    console.setCurrentFontIndex(font);

    //Init screen
    int32_t scr = console.createScreen();
    console.setCurrentScreenIndex(scr);

    auto& wm = console.getWidgetManager(scr);
    auto ti = wm.createWidget<conslr::widgets::TextInput>();
    {
        auto tiPtr = ti.lock();
        tiPtr->setRegion({ 0, 0, 40, 20 });
        tiPtr->setTitle("Enter text");
        tiPtr->showTitle();
        tiPtr->setString("This is a text input widget\nYou can edit the text here");

        wm.activateWidget(tiPtr->getId());
    }

    auto tb = wm.createWidget<conslr::widgets::TextBox>();
    {
        auto tbPtr = tb.lock();
        tbPtr->setRegion({ 40, 0, 40, 20 });
        tbPtr->setTitle("Stored text");
        tbPtr->showTitle();
    }

    //Made to track the efficacy of textinput
    //Mainly for my personal testing
    int32_t frames = 0;
    auto start = std::chrono::steady_clock::now();

    SDL_StartTextInput();

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

            if (event.type == SDL_TEXTINPUT)
            {
                ti.lock()->doTextInput(event.text);
            }

            if (event.type == SDL_KEYDOWN)
            {
                const auto& keys = console.getKeyMap();

                if (event.key.keysym == keys.scrollUp) { ti.lock()->doKeyUp(); }
                else if (event.key.keysym == keys.scrollDown) { ti.lock()->doKeyDown(); }
                else if (event.key.keysym == keys.scrollLeft) { ti.lock()->doKeyLeft(); }
                else if (event.key.keysym == keys.scrollRight) { ti.lock()->doKeyRight(); }
                else if (event.key.keysym == keys.backspace) { ti.lock()->doBackspace(); }
                else if (event.key.keysym == keys.tab) { ti.lock()->doTab(); }
                else if (event.key.keysym == keys.enter) { ti.lock()->doReturn(); }

                if (event.key.keysym.scancode == SDL_SCANCODE_LCTRL)
                {
                    tb.lock()->setString(ti.lock()->getString());
                }
            }

            console.doEvent(event);
        }

        console.render();

        //Approximate updates per second
        frames++;
        if (frames % 100 == 0)
        {
            auto current = std::chrono::steady_clock::now();
            std::cout << "Frames per second: " << (double)frames / (double)std::chrono::duration_cast<std::chrono::seconds>(current - start).count() << '\r';
            std::cout.flush();
        }
    }

    SDL_StopTextInput();

    std::cout << std::endl;

    console.destroy();
    SDL_Quit();

    return 0;
}
