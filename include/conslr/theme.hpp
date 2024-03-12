///
///@file conslr/theme.hpp
///@brief Defines the theme struct
///
#pragma once

#include <cstdint>

#include <SDL_pixels.h>

struct ExtendedTheme;

namespace conslr
{
    struct Theme
    {
        uint8_t borderHorizontal = '-'; //!<Horizontal border character
        uint8_t borderVertical = '|'; //!<Vertical border character
        uint8_t borderCornerTl = '+'; //!<Top left corner border character
        uint8_t borderCornerTr = '+'; //!<Top right corner border character
        uint8_t borderCornerBl = '+'; //!<Bottom left corner border character
        uint8_t borderCornerBr = '+'; //!<Bottom right corner border character
        uint8_t scrollbarCharacter = (11 * 16) + 10; //!<Scrollbar character
        uint8_t scrollbarCharacterHorizontal = (12 * 16) + 13; //!<Horizontal scrollbar character
        uint8_t selectionTick = 'X'; //!<Selected element tick
        uint8_t progress = (13 * 16) + 11; //!<Progress bar fill

        SDL_Color background = { 0, 0, 0, 255 }; //!<Background for widgets
        SDL_Color text = { 255, 255, 255, 255 }; //!<Text for widgets
        SDL_Color border = { 255, 255, 255, 255 }; //!<Border foreground for widgets
        SDL_Color selection = { 100, 100, 100, 255 }; //!<Selection background color for highlighting
                                                      
        ExtendedTheme* extended = nullptr; //!<User implemented theme
    };
}
