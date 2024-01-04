#include "conslr/console.hpp"

#include <iostream>
#include <cassert>

#include <SDL.h>
#include <SDL_image.h>

conslr::Console::Console(int32_t cellWidth, int32_t cellHeight, int32_t windowCellWidth, int32_t windowCellHeight) :
    mCellWidth{ cellWidth }, mCellHeight{ cellHeight },
    mWindowCellWidth{ windowCellWidth }, mWindowCellHeight{ windowCellHeight },
    mWindowWidth{ cellWidth * windowCellWidth }, mWindowHeight{ cellHeight * windowCellHeight },
    mWindow{ nullptr }, mRenderer{ nullptr },
    mCurrentScreen{ -1 },
    mCurrentFont{ -1 }
{}

void conslr::Console::init(const char* title, SDL_Surface* icon)
{
    //init was already called for this console or the object is somehow malformed
    if (mWindow != nullptr || mRenderer != nullptr)
    {
        std::cerr << "Console::init() was already called or this object is malformed" << std::endl;
        return;
    }

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
        exit(-2);
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer)
    {
        std::cerr << "Failed to create renderer for window with title: " << title << ", " << SDL_GetError() << std::endl;
        exit(-3);
    }

    if (icon != nullptr)
    {
        SDL_SetWindowIcon(mWindow, icon);
    }

    return;
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

    auto& scr = mScreens.at(mCurrentScreen);
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

    auto& scr = mScreens.at(mCurrentScreen);
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

    auto& scr = mScreens.at(mCurrentScreen);
    if (scr.render != nullptr && scr.mUpdated)
    {
        scr.render(scr);
        scr.mUpdated = false;
    }

    const auto& cells = mScreens.at(mCurrentScreen).getCells();
    //Ensure size of screen is same as size of console
    assert(cells.size() == mWindowCellWidth * mWindowCellHeight);
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
        const auto& font = mFonts.at(mCurrentFont);
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
    mScreens.clear();
    mCurrentScreen = -1;

    for (auto& font : mFonts)
    {
        if (font.mTexture)
        {
            SDL_DestroyTexture(font.mTexture);
        }
    }
    mFonts.clear();
    mCurrentFont = -1;

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
    mScreens.emplace_back(mWindowCellWidth, mWindowCellHeight);

    return mScreens.size() - 1;
}

//Font functions
int32_t conslr::Console::createFont(const char* file, int32_t charWidth, int32_t charHeight)
{
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

    assert(width % charWidth == 0);
    assert(height % charHeight == 0);

    Font font{ charWidth, charHeight, width / charWidth, height / charHeight, texture };
    mFonts.push_back(font);

    return mFonts.size() - 1;
}

//Getters
int32_t conslr::Console::getCurrentScreenIndex() const { return mCurrentScreen; }
int32_t conslr::Console::getCurrentFontIndex() const { return mCurrentFont; }

//Setters
void conslr::Console::setCurrentScreenIndex(int32_t index) { mCurrentScreen = index; }
void conslr::Console::setCurrentFontIndex(int32_t index) { mCurrentFont = index; }

void conslr::Console::setScreenEventCallback(int32_t index, std::function<void(Screen&, SDL_Event&)> callback) { mScreens.at(index).eventCallback = callback; }
void conslr::Console::setScreenUpdate(int32_t index, std::function<void(Screen&)> update) { mScreens.at(index).update = update; }
void conslr::Console::setScreenRender(int32_t index, std::function<void(Screen&)> render) { mScreens.at(index).render = render; }
