///
///@file conslr/console.hpp
///@brief Contains declarations for Console and Console::Font
///
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
        Console(int32_t cellWidth, int32_t cellHeight, int32_t windowCellWidth, int32_t windowCellHeight);
        ~Console();

        ///Initializes a console
        ///
        ///@param title Title of the console window
        ///@param icon Icon of the console window
        ///
        ///@return 0 on success, < 0 on failure
        int32_t init(const char* title = "Console", SDL_Surface* icon = nullptr);
        ///Processes events for the console
        ///
        ///@param event SDL_Event being passed to the console
        void doEvent(SDL_Event& event);
        ///Updates the current screen
        ///
        ///
        void update();
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
        int32_t createScreen();

        ///Creates a new font
        ///
        ///@param file Font sheet file to load
        ///@param charWidth Width of a character in the image file
        ///@param charHeight Height of a character in the image file
        ///
        ///@return Index of the new font
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
