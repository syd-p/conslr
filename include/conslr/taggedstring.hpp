///
///@file conslr/taggedstring.hpp
///@brief
///
#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include <SDL_pixels.h>

namespace conslr
{
    struct FgTag
    {
        int8_t index = 0;
    };

    struct BgTag
    {
        int8_t index = 0;
    };

    class TaggedString
    {
    public:
        struct TaggedChar
        {
            int8_t character;
            uint8_t tags;

            static const int8_t FOREGROUND_MASK = 0b11110000;
            static const int8_t BACKGROUND_MASK = 0b00001111;
        };

        TaggedString();

        TaggedString& operator<<(const std::string& str);
        TaggedString& operator<<(const char& c);
        TaggedString& operator<<(const FgTag& tag);
        TaggedString& operator<<(const BgTag& tag);

        void setTag(int32_t index, const SDL_Color& color);

        const std::vector<TaggedChar>& getString() const;
        const SDL_Color& getTag(int32_t index) const;

        static const int32_t MAX_TAGS = 16;

    private:
        std::vector<SDL_Color> mTags;
        std::vector<TaggedChar> mString;
        int8_t mCurrentForegroundTag;
        int8_t mCurrentBackgroundTag;
    };
}
