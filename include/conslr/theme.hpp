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
        int32_t borderHorizontal = '='; //!<Horizontal border character
        int32_t borderVertical = '|'; //!<Vertical border character
        int32_t borderCorner = '+'; //!<Corner border character
        int32_t scrollbarCharacter = (11 * 16) + 10; //!<Scrollbar character

        SDL_Color background = { 0, 0, 0, 255 }; //!<Background for widgets
        SDL_Color text = { 255, 255, 255, 255 }; //!<Text for widgets
        SDL_Color border = { 255, 255, 255, 255 }; //!<Border foreground for widgets
        SDL_Color selection = { 100, 100, 100, 255 }; //!<Selection background color for highlighting
    };
}
