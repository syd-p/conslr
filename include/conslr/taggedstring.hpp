///
///@file conslr/taggedstring.hpp
///@brief Declarations for TaggedString, TaggedString::TaggedChar and FgTag/BgTag
///
#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include <SDL_pixels.h>

namespace conslr
{
    ///
    ///Struct used for setting the index of the foreground color of a tagged string
    ///
    struct FgTag
    {
        int8_t index = 0; //!<Foreground index
    };

    ///
    ///Struct used for setting the index of the background color of a tagged string
    struct BgTag
    {
        int8_t index = 0; //!<Background index
    };

    ///
    ///Used for rendering tagged strings with Screen::renderTextTagged and Screen::renderMultilineTextTagged
    ///
    class TaggedString
    {
    public:
        ///
        ///Char type used by TaggedString
        ///
        struct TaggedChar
        {
            int8_t character; //!<Character of the string
            uint8_t tags; //!<Color tags of the string

            static const int8_t FOREGROUND_MASK = 0b11110000; //!<Mask for the foreground part of tags
            static const int8_t BACKGROUND_MASK = 0b00001111; //!<Mask for the background part of tags
        };

        ///
        ///Default constructor
        ///
        TaggedString();

        ///Inserts a string at the end of the tagged string
        ///
        ///@param str String to append
        ///@return TaggedString it was called on
        TaggedString& operator<<(const std::string& str);
        ///Inserts a char at the end of the tagged string
        ///
        ///@param c Character to append
        ///@return TaggedString it was called on
        TaggedString& operator<<(const char& c);
        ///Sets the foreground tag for the string
        ///
        ///@param tag Tag to set it to
        ///@return TaggedString it was called on
        TaggedString& operator<<(const FgTag& tag);
        ///Sets the background tag for the string
        ///
        ///@param tag Tag to set it to
        ///@return TaggedString it was called on
        TaggedString& operator<<(const BgTag& tag);

        ///Sets the color of a tag
        ///
        ///@param index Index of the tag, cannot be 0
        ///@param color Color to set tag to
        void setTag(int32_t index, const SDL_Color& color);

        ///Gets the string
        ///
        ///@return Vector of TaggedChar representing the string
        const std::vector<TaggedChar>& getString() const;
        ///Gets a tag
        ///
        ///@param index Index of the tag
        ///@return Color of the tag
        const SDL_Color& getTag(int32_t index) const;

        static const int32_t MAX_TAGS = 16; //!<Max tags a TaggedString can have (technically one more than max as 0 can not be used)

    private:
        std::vector<SDL_Color> mTags; //!<Vector of colors for tags
        std::vector<TaggedChar> mString; //!<Characters of the string
        int8_t mCurrentForegroundTag; //!<Current foreground tag
        int8_t mCurrentBackgroundTag; //!<Current background tag
    };
}
