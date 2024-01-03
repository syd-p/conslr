#include "conslr/console.hpp"

#include <iostream>

#include <SDL.h>

conslr::Console::Console(int32_t cellWidth, int32_t cellHeight, int32_t windowCellWidth, int32_t windowCellHeight) :
    mCellWidth{ cellWidth }, mCellHeight{ cellHeight },
    mWindowCellWidth{ windowCellWidth }, mWindowCellHeight{ windowCellHeight },
    mWindowWidth{ cellWidth * windowCellWidth }, mWindowHeight{ cellHeight * windowCellHeight },
    mWindow{ nullptr }, mRenderer{ nullptr }
{}

void conslr::Console::init(const char* title, SDL_Surface* icon)
{
    //init was already called for this console or the object is somehow malformed
    if (mWindow != nullptr || mRenderer != nullptr)
    {
        std::cerr << "Console::init() was already called or this object is malformed" << std::endl;
        return;
    }

    if (!SDL_WasInit(SDL_INIT_VIDEO))
    {
        if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
        {
            std::cerr << "Failed to init SDL video" << std::endl;
            exit(-1);
        }
    }

    mWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mWindowWidth, mWindowHeight, 0);
    if (!mWindow)
    {
        std::cerr << "Failed to create window with title: " << title << std::endl;
        exit(-2);
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer)
    {
        std::cerr << "Failed to create renderer for window with title: " << title << std::endl;
        exit(-3);
    }

    if (icon != nullptr)
    {
        SDL_SetWindowIcon(mWindow, icon);
    }

    return;
}

void conslr::Console::render() const
{
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
    SDL_RenderClear(mRenderer);

    //Render here

    SDL_RenderPresent(mRenderer);

    return;
}

void conslr::Console::destroy()
{
    if (mRenderer)
    {
        SDL_DestroyRenderer(mRenderer);
    }

    if (mWindow)
    {
        SDL_DestroyWindow(mWindow);
    }

    mRenderer = nullptr;
    mWindow = nullptr;

    return;
}
