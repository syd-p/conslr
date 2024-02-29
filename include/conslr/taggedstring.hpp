///
///@file conslr/taggedstring.hpp
///@brief Declarations for TaggedString, TaggedString::TaggedChar and FgTag/BgTag
///
#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>

#include <SDL_pixels.h>

namespace conslr
{
    struct TaggedChar
    {
        uint8_t character;
        uint8_t tags; //!<Color tags of the string
    };

    ///
    ///Wrapper class for std::vector<TaggedChar>
    ///
    struct TaggedString
    {
        constexpr TaggedString() noexcept {}
        ///Creates a tagged string from std::string
        ///
        ///@param str std::string to use
        ///@param fg Foreground tag
        ///@param bg Background tag
        constexpr TaggedString(const std::string& str, uint8_t fg, uint8_t bg) noexcept :
            str{ str.size() }
        {
            uint8_t tag = ((bg & 0x0F) << 4) | (fg & 0x0F);

            for (auto i = 0; i < str.size(); i++)
            {
                this->str.at(i).character = str.at(i);
                this->str.at(i).tags = tag;
            }

            return;
        }
        constexpr TaggedString(const TaggedString& other) : str{ other.str } {} //!<Copy constructor
        constexpr TaggedString(TaggedString&& other) noexcept : str{ std::move(other.str) } {} //!<Move constructor
        constexpr TaggedString& operator=(const TaggedString& other) { *this = TaggedString(other); return *this; } //!<Copy assignment
        constexpr TaggedString& operator=(TaggedString&& other) noexcept { str = std::move(other.str); return *this; } //!<Move assignment

        constexpr TaggedString operator+(const TaggedString& other)
        {
            str.insert(str.end(), other.str.begin(), other.str.end());

            return *this;
        }
        constexpr TaggedString& operator+=(const TaggedString& other)
        {
            str.insert(str.end(), other.str.begin(), other.str.end());

            return *this;
        }

        std::vector<TaggedChar> str;
    };

    const uint8_t FOREGROUND_MASK = 0x0F; //!<Mask for the foreground tag
    const uint8_t BACKGROUND_MASK = 0xF0; //!<Mask for the background tag

    using TagSet = std::array<SDL_Color, 16>;
}
