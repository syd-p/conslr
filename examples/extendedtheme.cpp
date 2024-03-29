///Example utilizing the extended theme functionality
///
///
#include <SDL.h>

#include "conslr/console.hpp"
#include "conslr/screen.hpp"
#include "conslr/theme.hpp"
#include "conslr/widget.hpp"

struct ExtendedTheme
{
    SDL_Color example = { 255, 0, 255, 255 };
};

class TestWidget : public conslr::IWidget, public conslr::IRenderable
{
public:
    constexpr TestWidget(int32_t id, int32_t priority) noexcept : IWidget{ id, priority } {}

    virtual void render(conslr::Screen& screen)
    {
        screen.fillBackground(mTheme->extended->example);

        return;
    }

private:
};

int main()
{
    conslr::Console console{ 16, 32, 80, 24 };
    console.setTitle("Extended Theme");

    int32_t scr = console.createScreen();
    console.setCurrentScreenIndex(scr);

    int32_t font = console.createFont("res/ibm_vga_fontsheet.bmp", 8, 16);
    console.setCurrentFontIndex(font);

    ExtendedTheme et;
    conslr::Theme newTheme;
    newTheme.extended = &et;
    console.setTheme(newTheme);

    auto& wm = console.getWidgetManager(scr);
    auto tw = wm.createWidget<TestWidget>();

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

            //Passes events to console
            console.doEvent(event);
        }

        console.render();
    }

    SDL_Quit();

    return 0;
}
