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
        int32_t borderHorizontal = '-'; //!<Horizontal border character
        int32_t borderVertical = '|'; //!<Vertical border character
        int32_t borderCornerTl = '+'; //!<Top left corner border character
        int32_t borderCornerTr = '+'; //!<Top right corner border character
        int32_t borderCornerBl = '+'; //!<Bottom left corner border character
        int32_t borderCornerBr = '+'; //!<Bottom right corner border character
        int32_t scrollbarCharacter = (11 * 16) + 10; //!<Scrollbar character
        int32_t scrollbarCharacterHorizontal = (12 * 16) + 13; //!<Horizontal scrollbar character
        int32_t selectionTick = 'X'; //!<Selected element tick
        int32_t progress = (13 * 16) + 11; //!<Progress bar fill

        SDL_Color background = { 0, 0, 0, 255 }; //!<Background for widgets
        SDL_Color text = { 255, 255, 255, 255 }; //!<Text for widgets
        SDL_Color border = { 255, 255, 255, 255 }; //!<Border foreground for widgets
        SDL_Color selection = { 100, 100, 100, 255 }; //!<Selection background color for highlighting
                                                      
        ExtendedTheme* extended = nullptr; //!<User implemented theme
    };
}
