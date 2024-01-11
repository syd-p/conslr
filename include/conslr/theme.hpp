///
///@file conslr/theme.hpp
///@brief
///
#pragma once

#include <cstdint>

#include <SDL_pixels.h>

namespace conslr
{
    ///Theme struct
    ///
    ///Provides a unified list data, but does not specify their usage by widgets
    struct Theme
    {
        int32_t borderHorizontal = '=';
        int32_t borderVertical = '|';
        int32_t borderCorner = '+';
        int32_t scrollbarCharacter = (11 * 16) + 10;

        SDL_Color background = { 0, 0, 0, 255 };
        SDL_Color text = { 255, 255, 255, 255 };
        SDL_Color border = { 255, 255, 255, 255 };
        SDL_Color selection = { 100, 100, 100, 255 };
    };
}
