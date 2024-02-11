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
        uint8_t character; //!<Character of the string
        uint8_t tags; //!<Color tags of the string
    };

    const uint8_t FOREGROUND_MASK = 0x0F;
    const uint8_t BACKGROUND_MASK = 0xF0;

    using TaggedString = std::vector<TaggedChar>;
    using TagSet = std::array<SDL_Color, 16>;

    inline TaggedString createTaggedString(const std::string& str, uint8_t fg, uint8_t bg)
    {
        TaggedString tmp{str.size()};

        uint8_t tag = ((bg & 0x0F) << 4) | (fg & 0x0F);

        for (auto i = 0; i < tmp.size(); i++)
        {
            tmp.at(i).character = str.at(i);
            tmp.at(i).tags = tag;
        }

        return tmp;
    }
}
