#pragma once

#include <cstdint>
#include <vector>

#include <SDL_events.h>

#include "conslr/screen.hpp"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;

namespace conslr
{
    class Console
    {
    public:
        Console() = delete;
        Console(const Console&) = delete;
        Console(Console&&) = delete;
        Console& operator=(const Console&) = delete;
        Console& operator=(Console&&) = delete;
        Console(int32_t cellWidth, int32_t cellHeight, int32_t windowCellWidth, int32_t windowCellHeight);

        void init(const char* title = "Console", SDL_Surface* icon = nullptr);
        void doEvent(SDL_Event& event);
        void update();
        void render();
        void destroy();

        //Screen functions
        int32_t createScreen();

        //Font functions
        int32_t createFont(const char* file, int32_t charWidth, int32_t charHeight);

        //Getters
        int32_t getCurrentScreenIndex() const;
        int32_t getCurrentFontIndex() const;

        //Setters
        void setCurrentScreenIndex(int32_t index);
        void setCurrentFontIndex(int32_t index);
        void setScreenEventCallback(int32_t index, std::function<void(Screen&, SDL_Event&)> callback);
        void setScreenUpdate(int32_t index, std::function<void(Screen&)> update);
        void setScreenRender(int32_t index, std::function<void(Screen&)> render);

    private:
        //Console data
        int32_t mCellWidth;
        int32_t mCellHeight;
        int32_t mWindowCellWidth;
        int32_t mWindowCellHeight;
        int32_t mWindowWidth;
        int32_t mWindowHeight;

        //SDL data
        SDL_Window* mWindow;
        SDL_Renderer* mRenderer;

        //Screen data
        std::vector<Screen> mScreens;
        int32_t mCurrentScreen;

        //Font data
        struct Font
        {
            int32_t mCharWidth;
            int32_t mCharHeight;
            int32_t mColumns;
            int32_t mRows;

            SDL_Texture* mTexture;
        };
        std::vector<Font> mFonts;
        int32_t mCurrentFont;
    };
}
