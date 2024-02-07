///
///@file conslr/keymapping.hpp
///@brief Defines the KeyMapping struct
///
#pragma once

#include <SDL_keyboard.h>

namespace conslr
{
    ///
    ///Pair used by KeyMapping
    ///
    struct KeyPair
    {
        SDL_Scancode scancode; //!<Key that is hit
        uint16_t mod; //!<Modifiers to the key
    };

    ///
    ///Collection of mappings for input
    ///
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

inline bool operator==(const conslr::KeyPair& keyPair, const SDL_Keysym& keySym)
{
    return (keyPair.scancode == keySym.scancode) && (keyPair.mod == keySym.mod);
}
