#include "conslr/screen.hpp"

#include <SDL_events.h>

#include "conslr/taggedstring.hpp"
#include "conslr/theme.hpp"

conslr::Screen::Screen(int32_t width, int32_t height) :
    mRerender{ true },
    mWidth{ width }, mHeight{ height },
    mCells{ (size_t)(mWidth * mHeight), Cell{} }
{}

void conslr::Screen::render()
{
    clear();

    for (auto& renderablePtr : mWidgetManager.getRenderable())
    {
        if (renderablePtr->isVisible())
        {
            renderablePtr->render(*this);
        }
        renderablePtr->mRerender = false;
    }

    mRerender = false;

    return;
}

void conslr::Screen::fill(const SDL_Color& background, const SDL_Color& foreground, const uint8_t& character)
{
    for (auto& cell : mCells)
    {
        cell.background = background;
        cell.foreground = foreground;
        cell.character = character;
    }

    mRerender = true;

    return;
}

void conslr::Screen::fillBackground(const SDL_Color& background)
{
    for (auto& cell : mCells)
    {
        cell.background = background;
    }

    mRerender = true;

    return;
}

void conslr::Screen::fillForeground(const SDL_Color& foreground)
{
    for (auto& cell : mCells)
    {
        cell.foreground = foreground;
    }

    mRerender = true;

    return;
}

void conslr::Screen::fillForeground(const SDL_Color& foreground, const uint8_t& character)
{
    for (auto& cell : mCells)
    {
        cell.foreground = foreground;
        cell.character = character;
    }

    mRerender = true;

    return;
}

void conslr::Screen::fillCharacter(const uint8_t& character)
{
    for (auto& cell : mCells)
    {
        cell.character = character;
    }

    mRerender = true;

    return;
}

void conslr::Screen::fillRect(SDL_Rect rect, const SDL_Color& background, const SDL_Color& foreground, const uint8_t& character)
{
    for (auto i = rect.x; i < rect.x + rect.w; i++)
    {
        if (i < 0 || i >= mWidth) { continue; }

        for (auto j = rect.y; j < rect.y + rect.h; j++)
        {
            if (j < 0 || j >= mHeight) { continue; }

            int32_t index = (j * mWidth) + i;
            auto& cell = mCells.at(index);
            cell.background = background;
            cell.foreground = foreground;
            cell.character = character;
        }
    }

    mRerender = true;

    return;
}

void conslr::Screen::fillRectBackground(SDL_Rect rect, const SDL_Color& background)
{
    for (auto i = rect.x; i < rect.x + rect.w; i++)
    {
        if (i < 0 || i >= mWidth) { continue; }

        for (auto j = rect.y; j < rect.y + rect.h; j++)
        {
            if (j < 0 || j >= mHeight) { continue; }

            int32_t index = (j * mWidth) + i;
            auto& cell = mCells.at(index);
            cell.background = background;
        }
    }

    mRerender = true;

    return;
}

void conslr::Screen::fillRectForeground(SDL_Rect rect, const SDL_Color& foreground)
{
    for (auto i = rect.x; i < rect.x + rect.w; i++)
    {
        if (i < 0 || i >= mWidth) { continue; }

        for (auto j = rect.y; j < rect.y + rect.h; j++)
        {
            if (j < 0 || j >= mHeight) { continue; }

            int32_t index = (j * mWidth) + i;
            auto& cell = mCells.at(index);
            cell.foreground = foreground;
        }
    }

    mRerender = true;

    return;
}

void conslr::Screen::fillRectForeground(SDL_Rect rect, const SDL_Color& foreground, const uint8_t& character)
{
    for (auto i = rect.x; i < rect.x + rect.w; i++)
    {
        if (i < 0 || i >= mWidth) { continue; }

        for (auto j = rect.y; j < rect.y + rect.h; j++)
        {
            if (j < 0 || j >= mHeight) { continue; }

            int32_t index = (j * mWidth) + i;
            auto& cell = mCells.at(index);
            cell.foreground = foreground;
            cell.character = character;
        }
    }

    mRerender = true;

    return;
}

void conslr::Screen::fillRectCharacter(SDL_Rect rect, const uint8_t& character)
{
    for (auto i = rect.x; i < rect.x + rect.w; i++)
    {
        if (i < 0 || i >= mWidth) { continue; }

        for (auto j = rect.y; j < rect.y + rect.h; j++)
        {
            if (j < 0 || j >= mHeight) { continue; }

            int32_t index = (j * mWidth) + i;
            auto& cell = mCells.at(index);
            cell.character = character;
        }
    }

    mRerender = true;

    return;
}

void conslr::Screen::setCell(int32_t x, int32_t y, const SDL_Color& background, const SDL_Color& foreground, const uint8_t& character)
{
    if (x < 0 || x >= mWidth || y < 0 || y >= mHeight) { return; }

    int32_t index = (y * mWidth) + x;
    auto& cell = mCells.at(index);
    cell.background = background;
    cell.foreground = foreground;
    cell.character = character;

    mRerender = true;

    return;
}

void conslr::Screen::setCellBackground(int32_t x, int32_t y, const SDL_Color& background)
{
    if (x < 0 || x >= mWidth || y < 0 || y >= mHeight) { return; }

    int32_t index = (y * mWidth) + x;
    auto& cell = mCells.at(index);
    cell.background = background;

    mRerender = true;

    return;
}

void conslr::Screen::setCellForeground(int32_t x, int32_t y, const SDL_Color& foreground)
{
    if (x < 0 || x >= mWidth || y < 0 || y >= mHeight) { return; }

    int32_t index = (y * mWidth) + x;
    auto& cell = mCells.at(index);
    cell.foreground = foreground;

    mRerender = true;

    return;
}

void conslr::Screen::setCellForeground(int32_t x, int32_t y, const SDL_Color& foreground, const uint8_t& character)
{
    if (x < 0 || x >= mWidth || y < 0 || y >= mHeight) { return; }

    int32_t index = (y * mWidth) + x;
    auto& cell = mCells.at(index);
    cell.foreground = foreground;
    cell.character = character;

    mRerender = true;

    return;
}

void conslr::Screen::setCellCharacter(int32_t x, int32_t y, const uint8_t& character)
{
    if (x < 0 || x >= mWidth || y < 0 || y >= mHeight) { return; }

    int32_t index = (y * mWidth) + x;
    auto& cell = mCells.at(index);
    cell.character = character;

    mRerender = true;

    return;
}

void conslr::Screen::borderRect(SDL_Rect rect, uint8_t horizontal, uint8_t vertical, uint8_t cornerTl, uint8_t cornerTr, uint8_t cornerBl, uint8_t cornerBr)
{
    for (auto i = rect.x; i < rect.x + rect.w; i++)
    {
        if (i < 0 || i >= mWidth) { continue; }

        int32_t index = (rect.y * mWidth) + i;
        int32_t index2 = ((rect.y + rect.h - 1) * mWidth) + i;

        if (rect.y >= 0 && rect.y < mHeight)
        {
            mCells.at(index).character = horizontal;
        }

        if (rect.y + rect.h - 1 >= 0 && rect.y + rect.h - 1 < mHeight)
        {
            mCells.at(index2).character = horizontal;
        }
    }

    for (auto j = rect.y; j < rect.y + rect.h; j++)
    {
        if (j < 0 || j >= mHeight) { continue; }

        int32_t index = (j * mWidth) + rect.x;
        int32_t index2 = (j * mWidth) + rect.x + rect.w - 1;

        if (rect.x >= 0 && rect.x < mWidth)
        {
            mCells.at(index).character = vertical;
        }

        if (rect.x + rect.w - 1 >= 0 && rect.x + rect.w - 1 < mWidth)
        {
            mCells.at(index2).character = vertical;
        }
    }

    int32_t tl = (rect.y * mWidth) + rect.x;
    int32_t bl = ((rect.y + rect.h - 1) * mWidth) + rect.x;

    if (rect.y >= 0 && rect.y < mHeight)
    {
        if (rect.x >= 0 && rect.x < mWidth)
        {
            mCells.at(tl).character = cornerTl;
        }

        if (rect.x + rect.w - 1 >= 0 && rect.x + rect.w - 1 < mWidth)
        {
            mCells.at(tl + rect.w - 1).character = cornerTr;
        }
    }

    if (rect.y + rect.h - 1 >= 0 && rect.y + rect.h - 1 < mHeight)
    {
        if (rect.x >= 0 && rect.x < mWidth)
        {
            mCells.at(bl).character = cornerBl;
        }

        if (rect.x + rect.w - 1 >= 0 && rect.x + rect.w - 1 < mWidth)
        {
            mCells.at(bl + rect.w - 1).character = cornerBr;
        }
    }

    mRerender = true;

    return;
}

void conslr::Screen::renderText(int32_t x, int32_t y, int32_t maxSize, const std::string& str)
{
    if (y < 0 || y >= mHeight) { return; }

    int32_t itrSize = std::min(maxSize, (int32_t)str.size());
    int32_t start = (y * mWidth) + x;
    for (int i = 0; i < itrSize; i++)
    {
        if (x + i < 0 || x + i >= mWidth) { continue; }

        mCells.at(start + i).character = (unsigned char)str.at(i);
    }

    mRerender = true;

    return;
}

void conslr::Screen::renderMultilineText(int32_t x, int32_t y, int32_t maxWidth, int32_t maxHeight, const std::string& str)
{
    int32_t i = 0;
    int32_t j = 0;
    for (const auto& c : str)
    {
        if (c == '\n')
        {
            i = 0;
            j++;
            continue;
        }

        if (i >= maxWidth)
        {
            i = 0;
            j++;
        }

        if (j >= maxHeight)
        {
            break;
        }

        if (x + i < 0 || x + i >= mWidth || y + j < 0 || y + j >= mHeight) { i++; continue; }

        mCells.at(((j + y) * mWidth) + x + i).character = (unsigned char)c;
        i++;
    }

    mRerender = true;

    return;
}

void conslr::Screen::renderTextColor(int32_t x, int32_t y, int32_t maxSize, const std::string& str, const SDL_Color& color)
{
    if (y < 0 || y >= mHeight) { return; }

    int32_t itrSize = std::min(maxSize, (int32_t)str.size());
    int32_t start = (y * mWidth) + x;
    for (int i = 0; i < itrSize; i++)
    {
        if (x + i < 0 || x + i >= mWidth) { continue; }

        auto& cell = mCells.at(start + i);

        cell.character = (unsigned char)str.at(i);
        cell.foreground = color;
    }

    mRerender = true;

    return;
}

void conslr::Screen::renderTextColor(int32_t x, int32_t y, int32_t maxSize, const std::string& str, const SDL_Color& foreground, const SDL_Color& background)
{
    if (y < 0 || y >= mHeight) { return; }

    int32_t itrSize = std::min(maxSize, (int32_t)str.size());
    int32_t start = (y * mWidth) + x;
    for (int i = 0; i < itrSize; i++)
    {
        if (x + i < 0 || x + i >= mWidth) { continue; }

        auto& cell = mCells.at(start + i);

        cell.character = (unsigned char)str.at(i);
        cell.foreground = foreground;
        cell.background = background;
    }

    mRerender = true;

    return;
}

void conslr::Screen::renderMultilineTextColor(int32_t x, int32_t y, int32_t maxWidth, int32_t maxHeight, const std::string& str, const SDL_Color& color)
{
    int32_t i = 0;
    int32_t j = 0;
    for (const auto& c : str)
    {
        if (c == '\n')
        {
            i = 0;
            j++;
            continue;
        }

        if (i >= maxWidth)
        {
            i = 0;
            j++;
        }

        if (j >= maxHeight)
        {
            break;
        }

        if (x + i < 0 || x + i >= mWidth || y + j < 0 || y + j >= mHeight) { i++; continue; }

        auto& cell = mCells.at(((j + y) * mWidth) + x + i);

        cell.character = (unsigned char)c;
        cell.foreground = color;

        i++;
    }

    mRerender = true;

    return;
}

void conslr::Screen::renderMultilineTextColor(int32_t x, int32_t y, int32_t maxWidth, int32_t maxHeight, const std::string& str, const SDL_Color& foreground, const SDL_Color& background)
{
    int32_t i = 0;
    int32_t j = 0;
    for (const auto& c : str)
    {
        if (c == '\n')
        {
            i = 0;
            j++;
            continue;
        }

        if (i >= maxWidth)
        {
            i = 0;
            j++;
        }

        if (j >= maxHeight)
        {
            break;
        }

        if (x + i < 0 || x + i >= mWidth || y + j < 0 || y + j >= mHeight) { i++; continue; }

        auto& cell = mCells.at(((j + y) * mWidth) + x + i);

        cell.character = (unsigned char)c;
        cell.foreground = foreground;
        cell.background = background;
        i++;
    }

    mRerender = true;

    return;
}

void conslr::Screen::renderTextTagged(int32_t x, int32_t y, int32_t maxSize, const TaggedString& str, const TagSet& tags)
{
    if (y < 0 || y >= mHeight) { return; }

    int32_t itrSize = std::min(maxSize, (int32_t)str.str.size());
    int32_t start = (y * mWidth) + x;
    for (int i = 0; i < itrSize; i++)
    {
        if (x + i < 0 || x + i >= mWidth) { continue; }

        auto& cell = mCells.at(start + i);
        //Current tagged character
        const auto& tc = str.str.at(i);
        //Current indexs of the color tags
        uint8_t bg = (tc.tags & TaggedChar::BACKGROUND_MASK) >> 4;
        uint8_t fg = tc.tags & TaggedChar::FOREGROUND_MASK;

        cell.character = tc.character;


        cell.foreground = tags.at(fg);
        cell.background = tags.at(bg);
    }

    mRerender = true;

    return;
}

void conslr::Screen::renderMultilineTextTagged(int32_t x, int32_t y, int32_t maxWidth, int32_t maxHeight, const TaggedString& str, const TagSet& tags)
{
    int32_t i = 0;
    int32_t j = 0;
    for (const auto& tc : str.str)
    {
        if (tc.character == '\n')
        {
            i = 0;
            j++;
            continue;
        }

        if (i >= maxWidth)
        {
            i = 0;
            j++;
        }

        if (j >= maxHeight)
        {
            break;
        }

        if (x + i < 0 || x + i >= mWidth || y + j < 0 || y + j >= mHeight) { i++; continue; }

        auto& cell = mCells.at(((j + y) * mWidth) + x + i);
        uint8_t bg = (tc.tags & TaggedChar::BACKGROUND_MASK) >> 4;
        uint8_t fg = tc.tags & TaggedChar::FOREGROUND_MASK;

        cell.character = tc.character;

        cell.foreground = tags.at(fg);
        cell.background = tags.at(bg);

        i++;
    }

    mRerender = true;

    return;
}

void conslr::Screen::clear()
{
    mCells.assign(mCells.size(), { mWidgetManager.mTheme->background, { 255, 255, 255, 255 }, 0 });

    return;
}
