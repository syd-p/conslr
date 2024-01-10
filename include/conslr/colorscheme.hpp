///
///@file conslr/colorscheme.hpp
///@brief
///
#pragma once

#include <cstdint>

#include <SDL_pixels.h>

namespace conslr
{
    ///Color Scheme struct
    ///
    ///Provides a unified list of colors, but does not specify their usage by widgets
    struct ColorScheme
    {
        int32_t borderHorizontal = '=';
        int32_t borderVertical = '|';
        int32_t borderCorner = '+';

        SDL_Color background = { 0, 0, 0, 255 };
        SDL_Color text = { 255, 255, 255, 255 };
        SDL_Color border = { 255, 255, 255, 255 };
        SDL_Color selection = { 100, 100, 100, 255 };
    };
}
