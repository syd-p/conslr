#include <SDL.h>

#include "conslr/console.hpp"

int main()
{
    conslr::Console console{ 32, 64, 80, 24 };
    if (console.init("Minimal Example") < 0)
    {
        exit(-1);
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
