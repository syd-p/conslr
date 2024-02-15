///
///@file conslr/console.hpp
///@brief Contains declarations for Console and Console::Font
///
#pragma once

#include <cstdint>
#include <array>
#include <queue>
#include <memory>

#include "conslr/keymapping.hpp"
#include "conslr/widgetmanager.hpp"
#include "conslr/theme.hpp"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;
union SDL_Event;

namespace conslr
{
    struct Screen;

    ///
    ///Console class
    ///
    class Console
    {
    public:
        Console() = delete;
        Console(const Console&) = delete;
        Console(Console&&) = delete;
        Console& operator=(const Console&) = delete;
        Console& operator=(Console&&) = delete;
        ///Constructs a console
        ///
        ///@param cellWidth Width of a cell
        ///@param cellHeight Height of a cell
        ///@param windowCellWidth Width of the console in cells
        ///@param windowCellHeight Height of the console in cells
        Console(int32_t cellWidth, int32_t cellHeight, int32_t windowCellWidth, int32_t windowCellHeight) noexcept;
        ///
        ///Destructor
        ///
        ~Console();

        ///Processes events for the console
        ///
        ///@param event SDL_Event being passed to the console
        void doEvent(SDL_Event& event);
        ///Renders the current screen
        ///
        ///Only called when screen.mUpdated is true
        void render();
        ///Destroys the console
        ///
        ///Console can then be reinitialized as if it were a newly constructed console
        void destroy();

        ///Creates a new screen
        ///
        ///@return Index of the new screen
        [[nodiscard]] int32_t createScreen();
        ///Destroys a screen
        ///
        ///@param index Index of screen
        void destroyScreen(int32_t index);

        ///Creates a new font
        ///
        ///@param file Font sheet file to load
        ///@param charWidth Width of a character in the image file
        ///@param charHeight Height of a character in the image file
        ///@return Index of the new font
        [[nodiscard]] int32_t createFont(const char* file, int32_t charWidth, int32_t charHeight);
        ///Destroys a font
        ///
        ///@param index Font to destroy
        void destroyFont(int32_t index);

        //Getters
        ///Gets the current screen index
        ///
        ///@return Index of the current screen
        [[nodiscard]] constexpr int32_t getCurrentScreenIndex() const noexcept { return mCurrentScreen; }
        ///Gets the current font index
        ///
        ///@return Index of the current font
        [[nodiscard]] constexpr int32_t getCurrentFontIndex() const noexcept { return mCurrentFont; }
        ///Returns the widget manager of a screen
        ///
        ///@param index Index of the screen
        ///@return Reference to the screen's WidgetManager
        [[nodiscard]] WidgetManager& getWidgetManager(int32_t index) const noexcept;
        ///Gets the current screen
        ///
        ///@return Reference to the current screen
        [[nodiscard]] constexpr const Theme& getTheme() const noexcept { return mTheme; }
        ///Gets the current key mapping
        ///
        ///@return Reference to the consoles key map
        [[nodiscard]] constexpr const KeyMapping& getKeyMap() const noexcept { return mKeyMap; }
        ///Gets the current window id
        ///
        ///@return Window id
        [[nodiscard]] constexpr int32_t getWindowId() const noexcept { return SDL_GetWindowID(mWindow); }

        //Setters
        ///Sets current screen index
        ///
        ///@param index Screen index
        constexpr void setCurrentScreenIndex(int32_t index) noexcept { assert(index >= 0 && index < mScreens.size()); mCurrentScreen = index; }
        ///Sets current font index
        ///
        ///@param index Font index
        constexpr void setCurrentFontIndex(int32_t index) noexcept { assert(index >= 0 && index < mFonts.size()); mCurrentFont = index; }
        ///Sets the theme of the console
        ///
        ///@param theme Theme to set as console theme
        void setTheme(const Theme& theme) noexcept;
        ///Sets the mapping of the console
        ///
        ///@param keyMap Mapping to set
        constexpr void setKeyMap(const KeyMapping& keyMap) noexcept { mKeyMap = keyMap; }
        constexpr void setTitle(const std::string& str) noexcept { SDL_SetWindowTitle(mWindow, str.c_str()); }
        constexpr void setIcon(SDL_Surface* icon) noexcept { SDL_SetWindowIcon(mWindow, icon); }

        //Const values
        static const int32_t MAX_SCREENS = 16; //!<Max screens that a console can have
        static const int32_t MAX_FONTS = 4; //!<Max fonts that a console can have

    private:
        //Console data
        int32_t mCellWidth;
        int32_t mCellHeight;
        int32_t mWindowCellWidth;
        int32_t mWindowCellHeight;
        int32_t mWindowWidth;
        int32_t mWindowHeight;
        Theme mTheme;

        //SDL data
        SDL_Window* mWindow;
        SDL_Renderer* mRenderer;
        KeyMapping mKeyMap;

        //Screen data
        std::queue<int32_t> mFreeScreens;
        std::array<std::unique_ptr<Screen>, MAX_SCREENS> mScreens;
        int32_t mCurrentScreen;

        //Font data
        struct Font
        {
            ~Font();

            int32_t mCharWidth = 0;
            int32_t mCharHeight = 0;
            int32_t mColumns = 0;
            int32_t mRows = 0;

            SDL_Texture* mTexture = nullptr;
        };
        std::queue<int32_t> mFreeFonts;
        std::array<std::unique_ptr<Font>, MAX_FONTS> mFonts;
        int32_t mCurrentFont;
    };
}
