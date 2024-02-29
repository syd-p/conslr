///
///@file conslr/screen.hpp
///@brief Contains declarations of Cell and Screen
///
#pragma once

#include <cstdint>
#include <vector>

#include <SDL_pixels.h>

#include <conslr/widgetmanager.hpp>
#include <conslr/taggedstring.hpp>

struct SDL_Renderer;
struct SDL_Rect;
union SDL_Event;

namespace conslr
{
    class Console;

    struct Cell
    {
        SDL_Color background = { 0, 0, 0, 255 }; //!<Background color
        SDL_Color foreground = { 255, 255, 255, 255 }; //!<Foreground color
        int32_t character = 0; //!<Character
    };

    class Screen
    {
    public:
        friend class conslr::Console;

        Screen() = delete;

        void render();

        void fill(const SDL_Color& background, const SDL_Color& foreground, const int32_t& character);
        void fillBackground(const SDL_Color& background);
        void fillForeground(const SDL_Color& foreground);
        void fillForeground(const SDL_Color& foreground, const int32_t& character);
        void fillCharacter(const int32_t& character);
        void fillRect(SDL_Rect rect, const SDL_Color& background, const SDL_Color& foreground, const int32_t& character);
        void fillRectBackground(SDL_Rect rect, const SDL_Color& background);
        void fillRectForeground(SDL_Rect rect, const SDL_Color& foreground);
        void fillRectForeground(SDL_Rect rect, const SDL_Color& foreground, const int32_t& character);
        void fillRectCharacter(SDL_Rect rect, const int32_t& character);
        void setCell(int32_t x, int32_t y, const SDL_Color& background, const SDL_Color& foreground, const int32_t& character);
        void setCellBackground(int32_t x, int32_t y, const SDL_Color& background);
        void setCellForeground(int32_t x, int32_t y, const SDL_Color& foreground);
        void setCellForeground(int32_t x, int32_t y, const SDL_Color& foreground, const int32_t& character);
        void setCellCharacter(int32_t x, int32_t y, const int32_t& character);
        ///Borders a rect (inclusive)
        ///
        ///@param rect Rect to border
        ///@param horizontal Horizontal character
        ///@param vertical Vertical character
        ///@param cornerTl Top left corner character
        ///@param cornerTr Top right corner character
        ///@param cornerBl Bottom left corner character
        ///@param cornerBr Bottom right corner character
        void borderRect(SDL_Rect rect, int32_t horizontal, int32_t vertical, int32_t cornerTl, int32_t cornerTr, int32_t cornerBl, int32_t cornerBr);
        ///Renders text to cells
        ///Only renders text on a single line, potentially wrapping if it overflows that line
        ///New line characters are interpreted as console int characters for this function
        ///Use renderMultilineText for multi-line text rendering
        ///
        ///@param x X index of the starting cell
        ///@param y Y index of the starting cell
        ///@param maxSize Maximum length to be rendered
        ///@param str String to be rendered
        void renderText(int32_t x, int32_t y, int32_t maxSize, const std::string& str);
        ///Renders a string across multiple lines
        ///
        ///@param x X index of the starting cell
        ///@param y Y index of the starting cell
        ///@param maxWidth Max width for a line
        ///@param maxHeight Max height for lines
        ///@param str String to be rendered
        void renderMultilineText(int32_t x, int32_t y, int32_t maxWidth, int32_t maxHeight, const std::string& str);
        ///Renders text to cells
        ///Only renders text on a single line, potentially wrapping if it overflows that line
        ///New line characters are interpreted as console int characters for this function
        ///Use renderMultilineText for multi-line text rendering
        ///
        ///@param x X index of the starting cell
        ///@param y Y index of the starting cell
        ///@param maxSize Maximum length to be rendered
        ///@param str String to be rendered
        ///@param color Text color
        void renderTextColor(int32_t x, int32_t y, int32_t maxSize, const std::string& str, const SDL_Color& color);
        ///Renders text to cells
        ///Only renders text on a single line, potentially wrapping if it overflows that line
        ///New line characters are interpreted as console int characters for this function
        ///Use renderMultilineText for multi-line text rendering
        ///
        ///@param x X index of the starting cell
        ///@param y Y index of the starting cell
        ///@param maxSize Maximum length to be rendered
        ///@param str String to be rendered
        ///@param foreground Text foreground color
        ///@param background Text background color
        void renderTextColor(int32_t x, int32_t y, int32_t maxSize, const std::string& str, const SDL_Color& foreground, const SDL_Color& background);
        ///Renders text to cells
        ///Only renders text on a single line, potentially wrapping if it overflows that line
        ///New line characters are interpreted as console int characters for this function
        ///Use renderMultilineText for multi-line text rendering
        ///
        ///@param x X index of the starting cell
        ///@param y Y index of the starting cell
        ///@param maxSize Maximum length to be rendered
        ///@param str TaggedString to be rendered
        ///@param tags Tags to use
        void renderTextTagged(int32_t x, int32_t y, int32_t maxSize, const TaggedString& str, const TagSet& tags);
        ///Renders a string across multiple lines
        ///
        ///@param x X index of the starting cell
        ///@param y Y index of the starting cell
        ///@param maxWidth Max width for a line
        ///@param maxHeight Max height for lines
        ///@param str String to be rendered
        ///@param color Text color
        void renderMultilineTextColor(int32_t x, int32_t y, int32_t maxWidth, int32_t maxHeight, const std::string& str, const SDL_Color& color);
        ///Renders a string across multiple lines
        ///
        ///@param x X index of the starting cell
        ///@param y Y index of the starting cell
        ///@param maxWidth Max width for a line
        ///@param maxHeight Max height for lines
        ///@param str String to be rendered
        ///@param foreground Text foreground color
        ///@param background Text background color
        void renderMultilineTextColor(int32_t x, int32_t y, int32_t maxWidth, int32_t maxHeight, const std::string& str, const SDL_Color& foreground, const SDL_Color& background);
        ///Renders a string across multiple lines
        ///
        ///@param x X index of the starting cell
        ///@param y Y index of the starting cell
        ///@param maxWidth Max width for a line
        ///@param maxHeight Max height for lines
        ///@param str TaggedString to be rendered
        ///@param tags Tags to use
        void renderMultilineTextTagged(int32_t x, int32_t y, int32_t maxWidth, int32_t maxHeight, const TaggedString& str, const TagSet& tags);

        //Getters
        [[nodiscard]] const int32_t& getWidth() const { return mWidth; }
        [[nodiscard]] const int32_t& getHeight() const { return mHeight; }
        [[nodiscard]] const std::vector<Cell>& getCells() const { return mCells; }

        bool mRerender; //!<Screen will be rerendered if true
        WidgetManager mWidgetManager;

    private:
        Screen(int32_t width, int32_t height);

        constexpr void clear();

        //Size in cells
        int32_t mWidth;
        int32_t mHeight;

        std::vector<Cell> mCells;
    };
}
