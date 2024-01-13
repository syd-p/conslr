///A minimal example of a conslr program
///
///Will simply create a console and display it
#include <SDL.h>

#include "conslr/console.hpp"

int main()
{
    //Creates console where cells are 32x64, and the console is 80x24 cells large
    conslr::Console console{ 32, 64, 80, 24 };
    if (console.init("Minimal Example") < 0)
    {
        //Console failed to init for some reason
        exit(-1);
    }

    //Normal SDL loop
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

        //Update and render
        console.update();
        console.render();
    }

    //Destroy console
    console.destroy();
    SDL_Quit();

    return 0;
}
