///
///@file conslr/themes/chameleon.hpp
///@brief Defines the Chameleon theme
///
#pragma once

#include "conslr/theme.hpp"

namespace conslr::themes
{
    ///
    ///Default theme
    ///
    static const Theme Chameleon
    {
        .borderHorizontal = 176, //!<Horizontal border character
        .borderVertical = 176, //!<Vertical border character
        .borderCornerTl = 176, //!<Top left corner border character
        .borderCornerTr = 176, //!<Top right corner border character
        .borderCornerBl = 176, //!<Bottom left corner border character
        .borderCornerBr = 176, //!<Bottom right corner border character
        .scrollbarCharacter = 219, //!<Scrollbar character
        .scrollbarCharacterHorizontal = 220, //!<Horizontal scrollbar character
        .selectionTick = 240, //!<Selected element tick
        .progress = 219,

        .background = { 10, 70, 10, 255 }, //!<Background for widgets
        .text = { 255, 255, 80, 255 }, //!<Text for widgets
        .border = { 255, 255, 80, 255 }, //!<Border foreground for widgets
        .selection = { 110, 120, 110, 255 } //!<Selection background color for highlighting
    };
}
