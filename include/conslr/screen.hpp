///
///@file conslr/screen.hpp
///@brief Contains declarations of Cell and Screen
///
#pragma once

#include <cstdint>
#include <vector>
#include <functional>

#include <SDL_pixels.h>
#include <SDL_events.h>

struct SDL_Renderer;
struct SDL_Rect;

namespace conslr
{
    ///
    ///Cell struct
    ///
    struct Cell
    {
        SDL_Color background = { 0, 0, 0, 255 }; //!<Background color
        SDL_Color foreground = { 255, 255, 255, 255 }; //!<Foreground color
        int32_t character = 0; //!<Character
    };

    ///Screen class
    ///
    ///Should not be created directly but instead through Console::createScreen();
    class Screen
    {
    public:
        Screen() = delete;
        ///Constructs a screen
        ///
        ///@param width Width in cells
        ///@param height Height in cells
        Screen(int32_t width, int32_t height);

        ///Callback function for events
        ///
        ///@param Screen& Screen that eventCallback is being called on
        ///@param SDL_Event& Event being passed to eventCallback
        std::function<void(Screen&, SDL_Event&)> eventCallback;
        ///Update function
        ///
        ///@param Screen& Screen that update is being called on
        std::function<void(Screen&)> update;
        ///Render function
        ///
        ///@param Screen& Screen that render is being called on
        std::function<void(Screen&)> render;

        //Drawing functions
        ///Fills the screen
        ///
        ///@param background Background color to fill
        ///@param foreground Foreground color to fill
        ///@param character Character to fill
        void fill(const SDL_Color& background, const SDL_Color& foreground, const int32_t& character);
        ///Fills the background
        ///
        ///@param background Background color to fill
        void fillBackground(const SDL_Color& background);
        ///Fills the foreground
        ///
        ///@param foreground Foreground color to fill
        void fillForeground(const SDL_Color& foreground);
        ///Fills the foreground
        ///
        ///@param foreground Foreground color to fill
        ///@param character Character to fill
        void fillForeground(const SDL_Color& foreground, const int32_t& character);
        ///Fills characters
        ///
        ///@param character Character to fill
        void fillCharacter(const int32_t& character);
        ///Fills a rect
        ///
        ///@param rect Rect to fill
        ///@param background Background color to fill
        ///@param foreground Foreground color to fill
        ///@param character Character to fill
        void fillRect(SDL_Rect rect, const SDL_Color& background, const SDL_Color& foreground, const int32_t& character);
        ///Fills the background of a rect
        ///
        ///@param rect Rect to fill
        ///@param background Background color to fill
        void fillRectBackground(SDL_Rect rect, const SDL_Color& background);
        ///Fills the foreground of a rect
        ///
        ///@param rect Rect to fill
        ///@param foreground Foreground color to fill
        void fillRectForeground(SDL_Rect rect, const SDL_Color& foreground);
        ///Fills the foreground of a rect
        ///
        ///@param rect Rect to fill
        ///@param foreground Foreground color to fill
        ///@param character Character to fill
        void fillRectForeground(SDL_Rect rect, const SDL_Color& foreground, const int32_t& character);
        ///Fills the characters in a rect
        ///
        ///@param rect Rect to fill
        ///@param character Character to fill
        void fillRectCharacter(SDL_Rect rect, const int32_t& character);

        //Getters
        const int32_t& getWidth() const { return mWidth; }
        const int32_t& getHeight() const { return mHeight; }
        const std::vector<Cell>& getCells() const { return mCells; }

        bool mUpdated; //!<Screen will be rerendered if true

    private:
        //Size in cells
        int32_t mWidth;
        int32_t mHeight;

        std::vector<Cell> mCells;
    };
}
