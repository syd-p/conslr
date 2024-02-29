///
///@file conslr/keymapping.hpp
///@brief Defines the KeyMapping struct
///
#pragma once

#include <SDL_keyboard.h>

namespace conslr
{
    struct KeyPair
    {
        SDL_Scancode scancode;
        uint16_t mod;

        ///Compares a KeyPair and Keysym
        ///Ignores the keycode of the SDL_Keysym as that is part of the physical keyboard, scancode however is abstracted
        ///
        ///@param keySym Keysym to compare
        ///@return True if scancodes are the same, otherwise false
        constexpr bool operator==(const SDL_Keysym& keySym) const noexcept
        {
            return (scancode == keySym.scancode) && (mod == keySym.mod);
        }
    };

    struct KeyMapping
    {
        KeyPair scrollUp = { SDL_SCANCODE_UP, KMOD_NONE }; //!<Scroll up
        KeyPair scrollDown = { SDL_SCANCODE_DOWN, KMOD_NONE }; //!<Scroll down
        KeyPair scrollLeft = { SDL_SCANCODE_LEFT, KMOD_NONE }; //!<Scroll left
        KeyPair scrollRight = { SDL_SCANCODE_RIGHT, KMOD_NONE }; //!<Scroll right

        KeyPair tab = { SDL_SCANCODE_TAB, KMOD_NONE }; //!<Tab
        KeyPair enter = { SDL_SCANCODE_RETURN, KMOD_NONE }; //!<Enter
        KeyPair backspace = { SDL_SCANCODE_BACKSPACE, KMOD_NONE }; //!<Backspace
    };
}

