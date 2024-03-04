///
///@file conslr/taggedstring.hpp
///@brief Declarations for TaggedString, TaggedString::TaggedChar and FgTag/BgTag
///
#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <sstream>
#include <cassert>

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

            for (size_t i = 0; i < str.size(); i++)
            {
                this->str.at(i).character = str.at(i);
                this->str.at(i).tags = tag;
            }

            return;
        }
        ///Creates a tagged string from a formatted std::string
        ///Tags in the string are marked by {[b/f]n} where b is background tag, f is foreground tag, and n is the tag index
        ///
        ///@param str String to process
        TaggedString(const std::string& str) noexcept
        {
            //Todo process string

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

    ///Creates a TagSet from a string
    ///The string should be in the format of "tagIndex R G B A"
    ///
    ///@param str String to process
    ///@return Created TagSet
    inline TagSet createTagSetFromString(const std::string& str)
    {
        TagSet tags;
            
        std::stringstream ss{ str };
        int32_t tagId;
        int32_t r;
        int32_t g;
        int32_t b;
        int32_t a;

        while (ss >> tagId >> r >> g >> b >> a)
        {
            assert((!ss.fail()) && "Failed to read tags");
            assert((tagId >= 0 && tagId < 16) && "Tag Id is too large");
            assert((r >= 0 && r < 256) && (g >= 0 && g < 256) && (b >= 0 && b < 256) && (a >= 0 && a < 256));

            tags.at(tagId) = { (uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)a };
        }

        return tags;
    }
}
