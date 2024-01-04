#include "conslr/screen.hpp"

conslr::Screen::Screen(int32_t width, int32_t height) :
    mWidth{ width }, mHeight{ height },
    eventCallback{ nullptr }, update{ nullptr }, render{ nullptr },
    mUpdated{ true }
{
    mCells.resize(mWidth * mHeight, {});

    return;
}

void conslr::Screen::fill(const SDL_Color& background, const SDL_Color& foreground, const int32_t& character)
{
    for (auto& cell : mCells)
    {
        cell.background = background;
        cell.foreground = foreground;
        cell.character = character;
    }

    return;
}
