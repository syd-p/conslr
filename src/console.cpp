#include "conslr/console.hpp"
#include "conslr/widgetmanager.hpp"

#include <iostream>
#include <cassert>

#include <SDL.h>
#include <SDL_image.h>

#include <conslr/screen.hpp>

conslr::Console::Console(int32_t cellWidth, int32_t cellHeight, int32_t windowCellWidth, int32_t windowCellHeight) :
    mCellWidth{ cellWidth }, mCellHeight{ cellHeight },
    mWindowCellWidth{ windowCellWidth }, mWindowCellHeight{ windowCellHeight },
    mWindowWidth{ cellWidth * windowCellWidth }, mWindowHeight{ cellHeight * windowCellHeight },
    mWindow{ nullptr }, mRenderer{ nullptr },
    mCurrentScreen{ -1 },
    mCurrentFont{ -1 }
{
    for (auto i = 0; i < MAX_SCREENS; i++)
    {
        mFreeScreens.push(i);
        mScreens.at(i).reset(nullptr);
    }

    for (auto i = 0; i < MAX_FONTS; i++)
    {
        mFreeFonts.push(i);
        mFonts.at(i).reset(nullptr);
    }

    return;
}

conslr::Console::~Console()
{
    destroy();

    return;
}

int32_t conslr::Console::init(const char* title, SDL_Surface* icon)
{
    //init was already called for this console or the object is somehow malformed
    assert((mWindow == nullptr && mRenderer == nullptr) && "Console was already initialized");

    if (!SDL_WasInit(SDL_INIT_VIDEO))
    {
        if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
        {
            std::cerr << "Failed to init SDL video: " << SDL_GetError() << std::endl;
            exit(-1);
        }
    }

    mWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mWindowWidth, mWindowHeight, 0);
    if (!mWindow)
    {
        std::cerr << "Failed to create window with title: " << title << ", " << SDL_GetError() << std::endl;
        mWindow = nullptr;

        return -2;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer)
    {
        std::cerr << "Failed to create renderer for window with title: " << title << ", " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
        mRenderer = nullptr;

        return -3;
    }

    if (icon != nullptr)
    {
        SDL_SetWindowIcon(mWindow, icon);
    }

    return 0;
}

void conslr::Console::doEvent(SDL_Event& event)
{
    if (event.type == SDL_QUIT)
    {
        destroy();
        return;
    }

    if (event.type == SDL_WINDOWEVENT)
    {
        if (event.window.windowID == SDL_GetWindowID(mWindow))
        {
            switch (event.window.event)
            {
            case SDL_WINDOWEVENT_CLOSE:
                destroy();
                return;
            default:
                return;
            }
        }
    }

    if (mCurrentScreen < 0)
    {
        return;
    }

    assert((mScreens.at(mCurrentScreen) != nullptr) && "Screen does not exist");
    auto& scr = *mScreens.at(mCurrentScreen);
    if (scr.eventCallback == nullptr)
    {
        return;
    }

    scr.eventCallback(scr, event);

    return;
}

void conslr::Console::update()
{
    if (mCurrentScreen < 0)
    {
        return;
    }

    assert((mScreens.at(mCurrentScreen) != nullptr) && "Screen does not exist");
    auto& scr = *mScreens.at(mCurrentScreen);
    if (scr.update == nullptr)
    {
        return;
    }

    scr.update(scr);

    return;
}

void conslr::Console::render()
{
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
    SDL_RenderClear(mRenderer);

    if (mCurrentScreen < 0)
    {
        SDL_RenderPresent(mRenderer);
        return;
    }

    assert((mScreens.at(mCurrentScreen) != nullptr) && "Screen does not exist");
    auto& scr = *mScreens.at(mCurrentScreen);

    for (auto& ptr : scr.mWidgetManager.getRenderable())
    {
        if (ptr->mRerender)
        {
            scr.mRerender = true;
            break;
        }
    }

    if (scr.mRerender)
    {
        scr.render();
        scr.mRerender = false;
    }

    const auto& cells = scr.getCells();
    //Ensure size of screen is same as size of console
    assert((cells.size() == mWindowCellWidth * mWindowCellHeight) && "Size of console and size of screen do not match");
    for (auto i = 0; i < mWindowCellWidth * mWindowCellHeight; i++)
    {
        const auto& cell = cells.at(i);
        int32_t indexX = i % mWindowCellWidth;
        int32_t indexY = i / mWindowCellWidth;
        SDL_Rect rect{ indexX * mCellWidth, indexY * mCellHeight, mCellWidth, mCellHeight };

        SDL_SetRenderDrawColor(mRenderer, cell.background.r, cell.background.g, cell.background.b, cell.background.a);
        SDL_RenderFillRect(mRenderer, &rect);

        if (mCurrentFont < 0)
        {
            continue;
        }
        const auto& font = *mFonts.at(mCurrentFont);
        SDL_Rect src{ (cell.character % font.mColumns) * font.mCharWidth, (cell.character / font.mColumns) * font.mCharHeight, font.mCharWidth, font.mCharHeight };

        SDL_SetTextureColorMod(font.mTexture, cell.foreground.r, cell.foreground.g, cell.foreground.b);
        SDL_SetTextureAlphaMod(font.mTexture, cell.foreground.a);
        SDL_RenderCopy(mRenderer, font.mTexture, &src, &rect);
    }

    SDL_RenderPresent(mRenderer);

    return;
}

void conslr::Console::destroy()
{
    for (auto& scrPtr : mScreens)
    {
        scrPtr.reset(nullptr);
    }
    mCurrentScreen = -1;
    mFreeScreens = {};
    for (auto i = 0; i < MAX_SCREENS; i++)
    {
        mFreeScreens.push(i);
    }

    for (auto& fontPtr : mFonts)
    {
        fontPtr.reset(nullptr);
    }
    mCurrentFont = -1;
    mFreeFonts = {};
    for (auto i = 0; i < MAX_FONTS; i++)
    {
        mFreeFonts.push(i);
    }

    if (mRenderer)
    {
        SDL_DestroyRenderer(mRenderer);
    }

    if (mWindow)
    {
        SDL_DestroyWindow(mWindow);
    }

    mRenderer = nullptr;
    mWindow = nullptr;

    return;
}

//Screen functions
int32_t conslr::Console::createScreen()
{
    assert((!mFreeScreens.empty()) && "Max number of screens created");

    int32_t index = mFreeScreens.front();
    mFreeScreens.pop();

    mScreens.at(index).reset(new Screen{ mWindowCellWidth, mWindowCellHeight });

    return index;
}

void conslr::Console::destroyScreen(int32_t index)
{
    assert((index >= 0 && index < MAX_SCREENS) && "Index is out of bounds");
    assert((mScreens.at(index) != nullptr) && "Screen is already nullptr");

    mScreens.at(index).reset(nullptr);
    mFreeScreens.push(index);

    return;
}

//Font functions
int32_t conslr::Console::createFont(const char* file, int32_t charWidth, int32_t charHeight)
{
    assert((!mFreeFonts.empty()) && "Max number of fonts created");

    if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
    {
        std::cerr << "Failed to init SDL_Image" << std::endl;
        return -1;
    }

    SDL_Texture* texture = IMG_LoadTexture(mRenderer, file);
    if (!texture)
    {
        std::cerr << "Failed to load font: " << file << std::endl;
        return -1;
    }

    int32_t width;
    int32_t height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    assert((width % charWidth == 0) && "Character width does not divide font evenly");
    assert((height % charHeight == 0) && "Character height does not divide font evenly");

    int32_t index = mFreeFonts.front();
    mFreeFonts.pop();
    mFonts.at(index).reset(new Font{ charWidth, charHeight, width / charWidth, height / charHeight, texture });

    return index;
}

void conslr::Console::destroyFont(int32_t index)
{
    assert((index >= 0 && index < MAX_FONTS) && "Index is out of bounds");
    assert((mFonts.at(index) != nullptr) && "Font is already nullptr");

    mFonts.at(index).reset(nullptr);
    mFreeFonts.push(index);

    return;
}

//Getters
int32_t conslr::Console::getCurrentScreenIndex() const { return mCurrentScreen; }
int32_t conslr::Console::getCurrentFontIndex() const { return mCurrentFont; }
conslr::WidgetManager& conslr::Console::getWidgetManager(int32_t index) const
{
    assert((index >= 0 && index < MAX_SCREENS) && "Index out of bounds");
    assert((mScreens.at(index) != nullptr) && "Screen does not exist");

    return mScreens.at(index)->mWidgetManager;
}

//Setters
void conslr::Console::setCurrentScreenIndex(int32_t index) { assert(index < mScreens.size()); mCurrentScreen = index; }
void conslr::Console::setCurrentFontIndex(int32_t index) { assert(index < mFonts.size()); mCurrentFont = index; }

void conslr::Console::setScreenEventCallback(int32_t index, std::function<void(Screen&, SDL_Event&)> callback) { mScreens.at(index)->eventCallback = callback; }
void conslr::Console::setScreenUpdate(int32_t index, std::function<void(Screen&)> update) { mScreens.at(index)->update = update; }

//Console::Font
conslr::Console::Font::~Font()
{
    if (mTexture != nullptr)
    {
        SDL_DestroyTexture(mTexture);
    }

    return;
}
