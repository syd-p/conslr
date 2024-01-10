///
///@file conslr/colorscheme.hpp
///@brief
///
#pragma once

#include <SDL_pixels.h>

namespace conslr
{
    struct ColorScheme
    {
        SDL_Color background = { 0, 0, 0, 255 };
        SDL_Color text = { 255, 255, 255, 255 };
        SDL_Color border = { 255, 255, 255, 255 };
        SDL_Color selection = { 100, 100, 100, 255 };
    };
}
