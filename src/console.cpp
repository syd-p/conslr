#include "conslr/console.hpp"

#include <iostream>
#include <cassert>

#include <SDL.h>
#include <SDL_image.h>

#include "conslr/screen.hpp"
#include "conslr/themes/defaulttheme.hpp"

conslr::Console::Console(int32_t cellWidth, int32_t cellHeight, int32_t windowCellWidth, int32_t windowCellHeight) noexcept :
    mCellWidth{ cellWidth }, mCellHeight{ cellHeight },
    mWindowCellWidth{ windowCellWidth }, mWindowCellHeight{ windowCellHeight },
    mWindowWidth{ cellWidth * windowCellWidth }, mWindowHeight{ cellHeight * windowCellHeight },
    mWindow{ nullptr }, mRenderer{ nullptr },
    mTheme{ themes::Default },
    mCurrentScreen{ -1 },
    mCurrentFont{ -1 }
{
    assert((cellWidth > 0 && cellHeight > 0 && windowCellWidth > 0 && windowCellHeight > 0) && "Console size is 0");

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

    if (!SDL_WasInit(SDL_INIT_VIDEO))
    {
        if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
        {
            std::cerr << "Failed to init SDL video: " << SDL_GetError() << std::endl;
            exit(-1);
        }
    }

    mWindow = SDL_CreateWindow("Console", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mWindowWidth, mWindowHeight, 0);
    if (!mWindow)
    {
        std::cerr << "Failed to create window, " << SDL_GetError() << std::endl;
        mWindow = nullptr;

        exit(-2);
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer)
    {
        std::cerr << "Failed to create renderer, " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
        mRenderer = nullptr;

        exit(-3);
    }

    return;
}

conslr::Console::Console(int32_t windowCellWidth, int32_t windowCellHeight, double widthPercent, double heightPercent) noexcept :
    mCellWidth{ 0 }, mCellHeight{ 0 },
    mWindowCellWidth{ windowCellWidth }, mWindowCellHeight{ windowCellHeight },
    mWindowWidth{ 0 }, mWindowHeight{ 0 },
    mWindow{ nullptr }, mRenderer{ nullptr },
    mTheme{ themes::Default },
    mCurrentScreen{ -1 },
    mCurrentFont{ -1 }
{
    assert((widthPercent > 0.0 && widthPercent <= 1.0) && "Invalid width percent");
    assert((heightPercent > 0.0 && heightPercent <= 1.0) && "Invalid height percent");

    if (!SDL_WasInit(SDL_INIT_VIDEO))
    {
        if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
        {
            std::cerr << "Failed to init SDL video: " << SDL_GetError() << std::endl;
            exit(-1);
        }
    }

    SDL_DisplayMode dm;
    if (SDL_GetCurrentDisplayMode(0, &dm) < 0)
    {
        std::cerr << SDL_GetError() << std::endl;
        exit(-4);
    }

    mWindowWidth = dm.w * widthPercent;
    mWindowHeight = dm.h * heightPercent;

    mCellWidth = std::max(1, mWindowWidth / windowCellWidth);
    mCellHeight = std::max(1, mWindowHeight / windowCellHeight);

    assert((mCellWidth > 0 && mCellHeight > 0 && windowCellWidth > 0 && windowCellHeight > 0) && "Console size is 0");

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

    mWindow = SDL_CreateWindow("Console", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mWindowWidth, mWindowHeight, 0);
    if (!mWindow)
    {
        std::cerr << "Failed to create window, " << SDL_GetError() << std::endl;
        mWindow = nullptr;

        exit(-2);
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer)
    {
        std::cerr << "Failed to create renderer, " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
        mRenderer = nullptr;

        exit(-3);
    }

    return;
}


conslr::Console::~Console()
{
    destroy();

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

    mScreens.at(index)->mWidgetManager.setTheme(&mTheme);

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

void conslr::Console::resizeCells(int32_t width, int32_t height) noexcept
{
    assert((width > 0 && height > 0) && "Cells can not be 0 wide/high");

    mCellWidth = width;
    mCellHeight = height;

    mWindowWidth = mCellWidth * mWindowCellWidth;
    mWindowHeight = mCellHeight * mWindowCellHeight;

    SDL_SetWindowSize(mWindow, mWindowWidth, mWindowHeight);

    return;
}

//Getters
conslr::WidgetManager& conslr::Console::getWidgetManager(int32_t index) const noexcept
{
    assert((index >= 0 && index < MAX_SCREENS) && "Index out of bounds");
    assert((mScreens.at(index) != nullptr) && "Screen does not exist");

    return mScreens.at(index)->mWidgetManager;
}

//Setters
void conslr::Console::setTheme(const Theme& theme) noexcept
{
    mTheme = theme;

    for (auto& scr : mScreens)
    {
        if (scr == nullptr)
        {
            continue;
        }

        scr->mRerender = true;
    }

    return;
}

//Console::Font
conslr::Console::Font::~Font()
{
    if (mTexture != nullptr)
    {
        SDL_DestroyTexture(mTexture);
    }

    return;
}
