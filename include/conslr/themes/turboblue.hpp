///
///@file conslr/themes/turboblue.hpp
///@brief Defines the turbo blue theme
///
#pragma once

#include "conslr/theme.hpp"

namespace conslr::themes
{
    static const Theme TurboBlue
    {
        .borderHorizontal = (12 * 16) + 13, //!<Horizontal border character
        .borderVertical = (11 * 16) + 10, //!<Vertical border character
        .borderCorner = (12 * 16) + 14, //!<Corner border character
        .scrollbarCharacter = (11 * 16) + 10, //!<Scrollbar character

        .background = { 0, 0, 175, 255 }, //!<Background for widgets
        .text = { 255, 255, 255, 255 }, //!<Text for widgets
        .border = { 255, 255, 255, 255 }, //!<Border foreground for widgets
        .selection = { 0, 168, 175, 255 } //!<Selection background color for highlighting
    };
}
