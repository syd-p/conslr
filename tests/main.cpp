#include "conslr/console.hpp"

#include <SDL.h>

int main(int argc, char* argv[])
{
    conslr::Console console(32, 64, 80, 24);
    console.init("TestConsole");

    int32_t font = console.createFont("ibm_vga_fontsheet.bmp", 8, 16);
    console.setCurrentFontIndex(font);

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
        }

        console.render();
    }

    console.destroy();
    SDL_Quit();

    return 0;
}
