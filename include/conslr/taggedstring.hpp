///
///@file conslr/taggedstring.hpp
///@brief Declarations for TaggedString, TaggedString::TaggedChar and FgTag/BgTag
///
#pragma once

#include <cstdint>
#include <stdexcept>
#include <vector>
#include <array>
#include <string>
#include <sstream>

#include <SDL_pixels.h>

namespace conslr
{
    struct TaggedChar
    {
        uint8_t character;
        uint8_t tags; 
    };

    const uint8_t FOREGROUND_MASK = 0x0F; //!<Mask for the foreground tag
    const uint8_t BACKGROUND_MASK = 0xF0; //!<Mask for the background tag

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
        constexpr TaggedString(const std::string& str, uint8_t fg, uint8_t bg) :
            str{ str.size() }
        {
            if ((fg > 0x0F) || (bg > 0x0F))
            {
                throw std::invalid_argument("Foreground and background tags must be in range 0-15, fg: " + std::to_string((int)fg) +  ", bg: " + std::to_string((int)bg));
            }

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
        ///Very rudimentary
        ///Example: "This is plain [f2][b3]This has a different foreground and background"
        ///
        ///@param formattedStr String to process
        TaggedString(std::string formattedStr)
        {
            //Todo: this but better somehow
            
            str.resize(formattedStr.size());

            //Initial tags, bg = 1, fg = 0
            uint8_t tag = (1 << 4) | (0 & FOREGROUND_MASK);

            for (size_t i = 0; i < str.size(); i++)
            {
                if (formattedStr.at(i) == '[')
                {
                    //Open tag

                    auto it = formattedStr.find(']', i);
                    if (it != formattedStr.size() && it - i <= 4)
                    {
                        std::string o{ formattedStr.begin() + i, formattedStr.begin() + it + 1 };

                        if (o.at(1) == 'f') 
                        {
                            uint8_t b = tag & BACKGROUND_MASK;
                            uint8_t f = std::stoi(std::string{ o.begin() + 2, o.end() - 1 }) & 0x0F;
                            tag = b | f;
                        }
                        else if (o.at(1) == 'b')
                        {
                            uint8_t b = (std::stoi(std::string{ o.begin() + 2, o.end() - 1 }) & 0x0F) << 4; 
                            uint8_t f = tag  & FOREGROUND_MASK;
                            tag = b | f;                          
                        }

                        formattedStr.erase(i, it - i + 1);                      
                        if (i != 0)
                        {
                            i--;
                        }
                    }
                }

                str.at(i).character = formattedStr.at(i);
                str.at(i).tags = tag;
            }

            str.resize(formattedStr.size());

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

        constexpr TaggedString& operator+=(const TaggedChar& other)
        {
            str.insert(str.end(), other);

            return *this;
        }

        friend constexpr TaggedString operator+(TaggedString tstr, const TaggedChar& tc)
        {
            tstr.str.insert(tstr.str.end(), tc);

            return tstr;
        }

        friend constexpr TaggedString operator+(const TaggedChar& tc, TaggedString tstr)
        {
            tstr.str.insert(tstr.str.begin(), tc);

            return tstr;
        }

        std::vector<TaggedChar> str;
    };

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
            if (ss.fail())
            {
                throw std::runtime_error("Failed to read tags, tags must be in the format of [id r g b a] as ints");
            }
            
            if (tagId > 0x0F)
            {
                throw std::runtime_error("Tag id must be between 0-15, id: " + std::to_string(tagId));
            }

            if (!((r >= 0 && r < 256) && (g >= 0 && g < 256) && (b >= 0 && b < 256) && (a >= 0 && a < 256)))
            {
                throw std::runtime_error("RGBA values must be between 0-255, r: " + std::to_string(r) + ", g: " + std::to_string(g) + ", b: " + std::to_string(b) + ", a: " + std::to_string(a));
            }

            tags.at(tagId) = { (uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)a };
        }

        return tags;
    }
}
