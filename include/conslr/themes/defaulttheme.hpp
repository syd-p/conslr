///
///@file conslr/themes/defaulttheme.hpp
///@brief Defines the default theme
///
#pragma once

#include "conslr/theme.hpp"

namespace conslr::themes
{
    ///
    ///Default theme
    ///
    static const Theme Default
    {
        .borderHorizontal = '-', //!<Horizontal border character
        .borderVertical = '|', //!<Vertical border character
        .borderCornerTl = '+', //!<Top left corner border character
        .borderCornerTr = '+', //!<Top right corner border character
        .borderCornerBl = '+', //!<Bottom left corner border character
        .borderCornerBr = '+', //!<Bottom right corner border character
        .scrollbarCharacter = (11 * 16) + 10, //!<Scrollbar character
        .scrollbarCharacterHorizontal = (12 * 16) + 13, //!<Horizontal scrollbar character
        .selectionTick = 'X', //!<Selected element tick

        .background = { 0, 0, 0, 255 }, //!<Background for widgets
        .text = { 255, 255, 255, 255 }, //!<Text for widgets
        .border = { 255, 255, 255, 255 }, //!<Border foreground for widgets
        .selection = { 100, 100, 100, 255 } //!<Selection background color for highlighting
    };
}
