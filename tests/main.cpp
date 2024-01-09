#include "conslr/console.hpp"
#include "conslr/screen.hpp"
#include "conslr/widget.hpp"

#include <chrono>
#include <iostream>

#include <SDL.h>

class RectWidget : public conslr::IWidget, public conslr::IRenderable
{
public:
    RectWidget(int32_t index, int32_t priority) : IWidget{ index, priority } {}
    virtual void render(conslr::Screen& scr) override
    {
        scr.fillRectBackground(r, color);
    }

    SDL_Rect r;
    SDL_Color color;
};

int main(int argc, char* argv[])
{
    const auto initStart{ std::chrono::steady_clock::now() };

    conslr::Console console(32, 64, 80, 24);
    if (console.init("TestConsole") < 0)
    {
        exit(-1);
    }

    int32_t font = console.createFont("ibm_vga_fontsheet.bmp", 8, 16);
    console.setCurrentFontIndex(font);

    int32_t scrIndex = console.createScreen();
    console.setCurrentScreenIndex(scrIndex);

    auto& wm = console.getWidgetManager(scrIndex);
    auto rect1 = wm.createWidget<RectWidget>(1);
    rect1.lock()->r = { 1, 1, 2, 2 };
    rect1.lock()->color = { 255, 0, 0, 255 };

    auto r2 = wm.createWidget<RectWidget>(0);
    r2.lock()->r = { 2, 2, 2, 2 };
    r2.lock()->color = { 0, 0, 255, 255 };

    /*for (auto i = 2; i < wm.MAX_WIDGETS; i++)
    {
        auto x = wm.createWidget<RectWidget>();
    }*/

    const auto initEnd{ std::chrono::steady_clock::now() };
    std::cout << std::chrono::duration<double>(initEnd - initStart) << std::endl;

    int32_t loopCount = 0;
    const auto start{ std::chrono::steady_clock::now() };

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
                if (event.key.keysym.scancode == SDL_SCANCODE_H)
                {
                    rect1.lock()->hide();
                }

                if (event.key.keysym.scancode == SDL_SCANCODE_S)
                {
                    rect1.lock()->show();
                }
            }

            console.doEvent(event);
        }

        console.update();
        console.render();

        loopCount++;
        if (loopCount == 60)
        {
            const auto end{ std::chrono::steady_clock::now() };
            std::cout << std::chrono::duration<double>(end - start) << std::endl;
        }
    }

    console.destroy();
    SDL_Quit();

    return 0;
}
