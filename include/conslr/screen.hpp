#pragma once

#include <cstdint>
#include <vector>
#include <functional>

#include <SDL_pixels.h>
#include <SDL_events.h>

struct SDL_Renderer;

namespace conslr
{
    struct Cell
    {
        SDL_Color background = { 0, 0, 0, 255 };
        SDL_Color foreground = { 255, 255, 255, 255 };
        int32_t character = 0;
    };

    class Screen
    {
    public:
        Screen() = delete;
        Screen(int32_t width, int32_t height);

        std::function<void(Screen&, SDL_Event&)> eventCallback;
        std::function<void(Screen&)> update;
        std::function<void(Screen&)> render;

        //Drawing functions
        void fill(const SDL_Color& background, const SDL_Color& foreground, const int32_t& character);

        //Getters
        const int32_t& getWidth() const { return mWidth; }
        const int32_t& getHeight() const { return mHeight; }
        const std::vector<Cell>& getCells() const { return mCells; }

        bool mUpdated;

    private:
        //Size in cells
        int32_t mWidth;
        int32_t mHeight;

        std::vector<Cell> mCells;
    };
}
