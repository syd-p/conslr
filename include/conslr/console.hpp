///
///@file conslr/console.hpp
///@brief Contains declarations for Console and Console::Font
///
#pragma once

#include <cstdint>
#include <array>
#include <queue>
#include <memory>
#include <stdexcept>
#include <string>

#include <SDL.h>

#include "conslr/keymapping.hpp"
#include "conslr/widgetmanager.hpp"
#include "conslr/theme.hpp"
#include "conslr/screen.hpp"

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
        ~Console();

        ///Process events for the console
        ///
        ///@return true is screen is still valid, false if screen was closed
        bool doEvent(SDL_Event& event);
        void render();

        [[nodiscard]] int32_t createScreen();
        void destroyScreen(int32_t index);

        [[nodiscard]] int32_t createFont(const std::string& file, int32_t charWidth, int32_t charHeight);
        void destroyFont(int32_t index);

        void resizeCells(int32_t width, int32_t height);

        //Getters
        [[nodiscard]] constexpr int32_t getCurrentScreenIndex() const noexcept { return mCurrentScreen; }
        [[nodiscard]] constexpr int32_t getCurrentFontIndex() const noexcept { return mCurrentFont; }
        [[nodiscard]] WidgetManager& getWidgetManager(int32_t index) const
        {
            if (!(index >= 0 && index < MAX_SCREENS))
            {
                throw std::invalid_argument("Screen index is out of bounds, index: " + std::to_string(index));
            }
            if (mScreens.at(index) == nullptr)
            {
                throw std::runtime_error("Screen at index is already nullptr, index: " + std::to_string(index));
            }

            return mScreens.at(index)->mWidgetManager;
        }
        [[nodiscard]] std::weak_ptr<Theme> getTheme() const noexcept { return mTheme; }
        [[nodiscard]] constexpr const KeyMapping& getKeyMap() const noexcept { return mKeyMap; }
        [[nodiscard]] int32_t getWindowId() const noexcept { return SDL_GetWindowID(mWindow.get()); }

        //Setters
        constexpr void setCurrentScreenIndex(int32_t index)
        { 
            if (!(index >= 0 && (size_t)index < mScreens.size())) { throw std::invalid_argument("Index is out of bounds, index: " + std::to_string(index)); } 
            mCurrentScreen = index; 
        }
        constexpr void setCurrentFontIndex(int32_t index) 
        { 
            if (!(index >= 0 && (size_t)index < mFonts.size())) { throw std::invalid_argument("Index is out of bounds, index: " + std::to_string(index)); } 
            mCurrentFont = index; 
        }
        void setTheme(const Theme& theme) noexcept;
        constexpr void setKeyMap(const KeyMapping& keyMap) noexcept { mKeyMap = keyMap; }
        void setTitle(const std::string& str) noexcept { SDL_SetWindowTitle(mWindow.get(), str.c_str()); }
        void setIcon(SDL_Surface* icon) noexcept { SDL_SetWindowIcon(mWindow.get(), icon); }

        //Const values
        static const int32_t MAX_SCREENS = 16; //!<Max screens that a console can have
        static const int32_t MAX_FONTS = 4; //!<Max fonts that a console can have

    private:
        ///Destroys the console
        ///
        ///Used for destroying a window when there are multiple windows
        void destroy();

        //Console data
        int32_t mCellWidth;
        int32_t mCellHeight;
        int32_t mWindowCellWidth;
        int32_t mWindowCellHeight;
        int32_t mWindowWidth;
        int32_t mWindowHeight;
        std::shared_ptr<Theme> mTheme;

        //SDL data
        std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> mWindow;
        std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> mRenderer;
        KeyMapping mKeyMap;

        //Screen data
        std::queue<int32_t> mFreeScreens;
        std::array<std::unique_ptr<Screen>, MAX_SCREENS> mScreens;
        int32_t mCurrentScreen;

        //Font data
        struct Font
        {
            Font(int32_t charW, int32_t charH, int32_t cols, int32_t rows) :
                mCharWidth{ charW }, mCharHeight{ charH },
                mColumns{ cols }, mRows{ rows },
                mTexture{ nullptr, SDL_DestroyTexture }
            {}

            int32_t mCharWidth = 0;
            int32_t mCharHeight = 0;
            int32_t mColumns = 0;
            int32_t mRows = 0;

            std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> mTexture; 
        };
        std::queue<int32_t> mFreeFonts;
        std::array<std::unique_ptr<Font>, MAX_FONTS> mFonts;
        int32_t mCurrentFont;
    };
}
