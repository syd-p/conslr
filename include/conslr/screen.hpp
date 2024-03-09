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

    class Screen
    {
    private:
        struct Cell
        {
            SDL_Color background = { 0, 0, 0, 255 };
            SDL_Color foreground = { 255, 255, 255, 255 };
            uint8_t character = 0;
        };

    public:
        friend class conslr::Console;

        Screen() = delete;

        void render();

        void fill(const SDL_Color& background, const SDL_Color& foreground, const uint8_t& character);
        void fillBackground(const SDL_Color& background);
        void fillForeground(const SDL_Color& foreground);
        void fillForeground(const SDL_Color& foreground, const uint8_t& character);
        void fillCharacter(const uint8_t& character);

        void fillRect(SDL_Rect rect, const SDL_Color& background, const SDL_Color& foreground, const uint8_t& character);
        void fillRectBackground(SDL_Rect rect, const SDL_Color& background);
        void fillRectForeground(SDL_Rect rect, const SDL_Color& foreground);
        void fillRectForeground(SDL_Rect rect, const SDL_Color& foreground, const uint8_t& character);
        void fillRectCharacter(SDL_Rect rect, const uint8_t& character);

        void setCell(int32_t x, int32_t y, const SDL_Color& background, const SDL_Color& foreground, const uint8_t& character);
        void setCellBackground(int32_t x, int32_t y, const SDL_Color& background);
        void setCellForeground(int32_t x, int32_t y, const SDL_Color& foreground);
        void setCellForeground(int32_t x, int32_t y, const SDL_Color& foreground, const uint8_t& character);
        void setCellCharacter(int32_t x, int32_t y, const uint8_t& character);

        void borderRect(SDL_Rect rect, uint8_t horizontal, uint8_t vertical, uint8_t cornerTl, uint8_t cornerTr, uint8_t cornerBl, uint8_t cornerBr);

        void renderText(int32_t x, int32_t y, int32_t maxSize, const std::string& str);
        void renderMultilineText(int32_t x, int32_t y, int32_t maxWidth, int32_t maxHeight, const std::string& str);
        void renderTextColor(int32_t x, int32_t y, int32_t maxSize, const std::string& str, const SDL_Color& color);
        void renderTextColor(int32_t x, int32_t y, int32_t maxSize, const std::string& str, const SDL_Color& foreground, const SDL_Color& background);
        void renderTextTagged(int32_t x, int32_t y, int32_t maxSize, const TaggedString& str, const TagSet& tags);
        void renderMultilineTextColor(int32_t x, int32_t y, int32_t maxWidth, int32_t maxHeight, const std::string& str, const SDL_Color& color);
        void renderMultilineTextColor(int32_t x, int32_t y, int32_t maxWidth, int32_t maxHeight, const std::string& str, const SDL_Color& foreground, const SDL_Color& background);
        void renderMultilineTextTagged(int32_t x, int32_t y, int32_t maxWidth, int32_t maxHeight, const TaggedString& str, const TagSet& tags);

        //Getters
        [[nodiscard]] const int32_t& getWidth() const { return mWidth; }
        [[nodiscard]] const int32_t& getHeight() const { return mHeight; }
        [[nodiscard]] const std::vector<Cell>& getCells() const { return mCells; }

        bool mRerender;
        WidgetManager mWidgetManager;

    private:
        Screen(int32_t width, int32_t height);

        void clear();

        //Size in cells
        int32_t mWidth;
        int32_t mHeight;

        std::vector<Cell> mCells;
    };
}
