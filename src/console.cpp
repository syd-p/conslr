#include "conslr/console.hpp"

#include <cassert>
#include <stdexcept>
#include <string>
#include <fstream>
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>

#include "conslr/screen.hpp"
#include "theme.hpp"

conslr::Console::Console(int32_t cellWidth, int32_t cellHeight, int32_t windowCellWidth, int32_t windowCellHeight) :
    mCellWidth{ cellWidth }, mCellHeight{ cellHeight },
    mWindowCellWidth{ windowCellWidth }, mWindowCellHeight{ windowCellHeight },
    mWindowWidth{ cellWidth * windowCellWidth }, mWindowHeight{ cellHeight * windowCellHeight },
    mTheme{ std::make_shared<Theme>() },
    mWindow{ nullptr, SDL_DestroyWindow }, mRenderer{ nullptr, SDL_DestroyRenderer },
    mCurrentScreen{ -1 },
    mCurrentFont{ -1 }
{
    if (!(cellWidth > 0 && cellHeight > 0 && windowCellWidth > 0 && windowCellHeight > 0))
    {
        throw std::invalid_argument("Console sizes must be at least 1, cellWidth: " + std::to_string(cellWidth) +
                ", cellHeight: " + std::to_string(cellHeight) +
                ", windowCellWidth: " + std::to_string(windowCellWidth) +
                ", windowCellHeight: " + std::to_string(windowCellHeight));
    }

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
            throw std::runtime_error(std::string("Failed to init video: ") + SDL_GetError());
        }
    }

    mWindow.reset(SDL_CreateWindow("Console", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mWindowWidth, mWindowHeight, 0));
    if (!mWindow)
    {
        throw std::runtime_error(std::string("Failed to create window: ") + SDL_GetError());
    }

    mRenderer.reset(SDL_CreateRenderer(mWindow.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
    if (!mRenderer)
    {
        throw std::runtime_error(std::string("Failed to create renderer: ") + SDL_GetError());
    }

    return;
}

conslr::Console::~Console()
{
    destroy();

    return;
}

bool conslr::Console::doEvent(SDL_Event& event)
{
    if (event.type == SDL_QUIT)
    {
        destroy();
        return false;
    }

    if (event.type == SDL_WINDOWEVENT)
    {
        if (event.window.windowID == SDL_GetWindowID(mWindow.get()))
        {
            switch (event.window.event)
            {
            case SDL_WINDOWEVENT_CLOSE:
                destroy();
                return false;
            default:
                return true;
            }
        }
    }

    return true;
}

void conslr::Console::render()
{
    SDL_SetRenderDrawColor(mRenderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(mRenderer.get());

    if (mCurrentScreen < 0)
    {
        SDL_RenderPresent(mRenderer.get());
        return;
    }

    if (mScreens.at(mCurrentScreen) == nullptr)
    {
        throw std::runtime_error("Current screen is nullptr, currentScreen: " + std::to_string(mCurrentScreen));
    }
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
    assert((cells.size() == (size_t)(mWindowCellWidth * mWindowCellHeight)) && "Size of console and size of screen do not match");
    for (auto i = 0; i < mWindowCellWidth * mWindowCellHeight; i++)
    {
        const auto& cell = cells.at(i);
        int32_t indexX = i % mWindowCellWidth;
        int32_t indexY = i / mWindowCellWidth;
        SDL_Rect rect{ indexX * mCellWidth, indexY * mCellHeight, mCellWidth, mCellHeight };

        SDL_SetRenderDrawColor(mRenderer.get(), cell.background.r, cell.background.g, cell.background.b, cell.background.a);
        SDL_RenderFillRect(mRenderer.get(), &rect);

        if (mCurrentFont < 0)
        {
            continue;
        }
        const auto& font = *mFonts.at(mCurrentFont);
        SDL_Rect src{ (cell.character % font.mColumns) * font.mCharWidth, (cell.character / font.mColumns) * font.mCharHeight, font.mCharWidth, font.mCharHeight };

        SDL_SetTextureColorMod(font.mTexture.get(), cell.foreground.r, cell.foreground.g, cell.foreground.b);
        SDL_SetTextureAlphaMod(font.mTexture.get(), cell.foreground.a);
        SDL_RenderCopy(mRenderer.get(), font.mTexture.get(), &src, &rect);
    }

    SDL_RenderPresent(mRenderer.get());

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
        mRenderer.reset(nullptr);
    }

    if (mWindow)
    {
        mWindow.reset(nullptr);
    }

    return;
}

//Screen functions
int32_t conslr::Console::createScreen()
{
    if (mFreeScreens.empty())
    {
        throw std::runtime_error("Max screens created, max: " + std::to_string(MAX_SCREENS));
    }

    int32_t index = mFreeScreens.front();
    mFreeScreens.pop();

    mScreens.at(index).reset(new Screen{ mWindowCellWidth, mWindowCellHeight });

    mScreens.at(index)->mWidgetManager.setTheme(mTheme);

    return index;
}

void conslr::Console::destroyScreen(int32_t index)
{
    if (!(index >= 0 && index < MAX_SCREENS))
    {
        throw std::invalid_argument("Screen index is out of bounds, index: " + std::to_string(index));
    }
    if (mScreens.at(index) == nullptr)
    {
        throw std::runtime_error("Screen at index is already nullptr, index: " + std::to_string(index));
    }

    mScreens.at(index).reset(nullptr);
    mFreeScreens.push(index);

    return;
}

//Font functions
int32_t conslr::Console::createFont(const std::string& file, int32_t charWidth, int32_t charHeight)
{
    if (mFreeFonts.empty())
    {
        throw std::runtime_error("Max fonts created, max: " + std::to_string(MAX_FONTS));
    }

    if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
    {
        throw std::runtime_error("Failed to init SDL2_image");
    }

    SDL_Texture* texture = IMG_LoadTexture(mRenderer.get(), file.c_str());
    if (!texture)
    {
        throw std::runtime_error(std::string("Failed to load image, file: ") + file);
    }

    int32_t width;
    int32_t height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    if ((width % charWidth != 0 || height % charHeight != 0))
    {
        throw std::runtime_error("Character width or height does not divide image evenly, charWidth: " + std::to_string(charWidth) + ", width: " + std::to_string(width)  +
                ", charHeight: " + std::to_string(charHeight) + ", height: " + std::to_string(height));
    }

    int32_t index = mFreeFonts.front();
    mFreeFonts.pop();
    mFonts.at(index).reset(new Font{ charWidth, charHeight, width / charWidth, height / charHeight });
    mFonts.at(index)->mTexture.reset(texture);

    return index;
}

void conslr::Console::destroyFont(int32_t index)
{
    if (!(index >= 0 && index < MAX_FONTS))
    {
        throw std::invalid_argument("Font index is out of bounds, index: " + std::to_string(index));
    }
    if (mFonts.at(index) == nullptr)
    {
        throw std::runtime_error("Font at index is already nullptr, index: " + std::to_string(index));
    }

    mFonts.at(index).reset(nullptr);
    mFreeFonts.push(index);

    return;
}

conslr::Theme conslr::Console::createTheme(const std::string& file)
{
    std::ifstream ifs(file);
    if (!ifs.good())
    {
        ifs.close();
        std::cerr << "Failed to open file: " << file << std::endl;
        return Theme{};
    }

    conslr::Theme theme;
    std::string line;
    while (std::getline(ifs, line))
    {
        std::stringstream ss{line};
        std::string param;
        std::string value;

        std::getline(ss, param, '=');
        std::getline(ss, value);

        //Process params and values
        if (param == "borderHorizontal")
        {
            if (value.at(0) == '\'')
            {
                theme.borderHorizontal = value.at(1);
                continue;
            }

            theme.borderHorizontal = std::stoi(value);
            continue;
        }

        if (param == "borderVertical")
        {
            if (value.at(0) == '\'')
            {
                theme.borderVertical = value.at(1);
                continue;
            }

            theme.borderVertical = std::stoi(value);
            continue;
        }

        if (param == "borderCornerTl")
        {
            if (value.at(0) == '\'')
            {
                theme.borderCornerTl = value.at(1);
                continue;
            }

            theme.borderCornerTl = std::stoi(value);
            continue;
        }

        if (param == "borderCornerTr")
        {
            if (value.at(0) == '\'')
            {
                theme.borderCornerTr = value.at(1);
                continue;
            }

            theme.borderCornerTr = std::stoi(value);
            continue;
        }

        if (param == "borderCornerBl")
        {
            if (value.at(0) == '\'')
            {
                theme.borderCornerBl= value.at(1);
                continue;
            }

            theme.borderCornerBl = std::stoi(value);
            continue;
        }

        if (param == "borderCornerBr")
        {
            if (value.at(0) == '\'')
            {
                theme.borderCornerBr = value.at(1);
                continue;
            }

            theme.borderCornerBr = std::stoi(value);
            continue;
        }

        if (param == "scrollbarCharacter")
        {
            if (value.at(0) == '\'')
            {
                theme.scrollbarCharacter = value.at(1);
                continue;
            }

            theme.scrollbarCharacter = std::stoi(value);
            continue;
        }

        if (param == "scrollbarCharacterHorizontal")
        {
            if (value.at(0) == '\'')
            {
                theme.scrollbarCharacterHorizontal = value.at(1);
                continue;
            }

            theme.scrollbarCharacterHorizontal = std::stoi(value);
            continue;
        }

        if (param == "selectionTick")
        {
            if (value.at(0) == '\'')
            {
                theme.selectionTick = value.at(1);
                continue;
            }

            theme.selectionTick = std::stoi(value);
            continue;
        }

        if (param == "progress")
        {
            if (value.at(0) == '\'')
            {
                theme.progress = value.at(1);
                continue;
            }

            theme.progress = std::stoi(value);
            continue;
        }

        if (param == "background")
        {
            int32_t r;
            int32_t g;
            int32_t b;
            int32_t a;
            
            std::stringstream colors{value};
            colors >> r >> g >> b >> a;
            
            theme.background = { (uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)a };
            continue;
        }

        if (param == "text")
        {
            int32_t r;
            int32_t g;
            int32_t b;
            int32_t a;
            
            std::stringstream colors{value};
            colors >> r >> g >> b >> a;
            
            theme.text = { (uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)a };
            continue;
        }

        if (param == "border")
        {
            int32_t r;
            int32_t g;
            int32_t b;
            int32_t a;
            
            std::stringstream colors{value};
            colors >> r >> g >> b >> a;
            
            theme.border = { (uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)a };
            continue;
        }

        if (param == "selection")
        {
            int32_t r;
            int32_t g;
            int32_t b;
            int32_t a;
            
            std::stringstream colors{value};
            colors >> r >> g >> b >> a;
            
            theme.selection = { (uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)a };
            continue;
        }
    }

    ifs.close();
    return theme;
}

void conslr::Console::resizeCells(int32_t width, int32_t height) 
{
    if (width <= 0 || height <= 0)
    {
        throw std::invalid_argument("Width and height must be greater than 0, width: " + std::to_string(width) + ", height: " + std::to_string(height));
    }

    mCellWidth = width;
    mCellHeight = height;

    mWindowWidth = mCellWidth * mWindowCellWidth;
    mWindowHeight = mCellHeight * mWindowCellHeight;

    SDL_SetWindowSize(mWindow.get(), mWindowWidth, mWindowHeight);

    return;
}

//Setters
void conslr::Console::setTheme(const Theme& theme) noexcept
{
    (*mTheme) = theme;

    for (auto& scr : mScreens)
    {
        if (scr == nullptr)
        {
            continue;
        }

        scr->mWidgetManager.setTheme(mTheme);

        scr->mRerender = true;
    }

    return;
}

