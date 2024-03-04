///A minimal example of a conslr program
///
///Will simply create a console and display it
#include <SDL.h>

#include "conslr/console.hpp"

int main()
{
    //Init console
    conslr::Console console{ 16, 32, 80, 24 };
    console.setTitle("Minimal");

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

        console.render();
    }

    //Destroy console
    console.destroy();
    SDL_Quit();

    return 0;
}
