#include "conslr/screen.hpp"

#include <SDL_events.h>

#include "conslr/taggedstring.hpp"
#include "conslr/theme.hpp"

conslr::Screen::Screen(int32_t width, int32_t height) :
    mWidth{ width }, mHeight{ height },
    mRerender{ true }
{
    mCells.resize(mWidth * mHeight, {});

    return;
}

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

void conslr::Screen::fill(const SDL_Color& background, const SDL_Color& foreground, const int32_t& character)
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

void conslr::Screen::fillForeground(const SDL_Color& foreground, const int32_t& character)
{
    for (auto& cell : mCells)
    {
        cell.foreground = foreground;
        cell.character = character;
    }

    mRerender = true;

    return;
}

void conslr::Screen::fillCharacter(const int32_t& character)
{
    for (auto& cell : mCells)
    {
        cell.character = character;
    }

    mRerender = true;

    return;
}

void conslr::Screen::fillRect(SDL_Rect rect, const SDL_Color& background, const SDL_Color& foreground, const int32_t& character)
{
    if (rect.x < 0) { rect.x = 0; }
    if (rect.y < 0) { rect.y = 0; }
    if (rect.x + rect.w > mWidth) { rect.w = mWidth - rect.x; }
    if (rect.y + rect.h > mHeight) { rect.h = mHeight - rect.y; }

    for (auto i = rect.x; i < rect.x + rect.w; i++)
    {
        for (auto j = rect.y; j < rect.y + rect.h; j++)
        {
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
    if (rect.x < 0) { rect.x = 0; }
    if (rect.y < 0) { rect.y = 0; }
    if (rect.x + rect.w > mWidth) { rect.w = mWidth - rect.x; }
    if (rect.y + rect.h > mHeight) { rect.h = mHeight - rect.y; }

    for (auto i = rect.x; i < rect.x + rect.w; i++)
    {
        for (auto j = rect.y; j < rect.y + rect.h; j++)
        {
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
    if (rect.x < 0) { rect.x = 0; }
    if (rect.y < 0) { rect.y = 0; }
    if (rect.x + rect.w > mWidth) { rect.w = mWidth - rect.x; }
    if (rect.y + rect.h > mHeight) { rect.h = mHeight - rect.y; }

    for (auto i = rect.x; i < rect.x + rect.w; i++)
    {
        for (auto j = rect.y; j < rect.y + rect.h; j++)
        {
            int32_t index = (j * mWidth) + i;
            auto& cell = mCells.at(index);
            cell.foreground = foreground;
        }
    }

    mRerender = true;

    return;
}

void conslr::Screen::fillRectForeground(SDL_Rect rect, const SDL_Color& foreground, const int32_t& character)
{
    if (rect.x < 0) { rect.x = 0; }
    if (rect.y < 0) { rect.y = 0; }
    if (rect.x + rect.w > mWidth) { rect.w = mWidth - rect.x; }
    if (rect.y + rect.h > mHeight) { rect.h = mHeight - rect.y; }

    for (auto i = rect.x; i < rect.x + rect.w; i++)
    {
        for (auto j = rect.y; j < rect.y + rect.h; j++)
        {
            int32_t index = (j * mWidth) + i;
            auto& cell = mCells.at(index);
            cell.foreground = foreground;
            cell.character = character;
        }
    }

    mRerender = true;

    return;
}

void conslr::Screen::fillRectCharacter(SDL_Rect rect, const int32_t& character)
{
    if (rect.x < 0) { rect.x = 0; }
    if (rect.y < 0) { rect.y = 0; }
    if (rect.x + rect.w > mWidth) { rect.w = mWidth - rect.x; }
    if (rect.y + rect.h > mHeight) { rect.h = mHeight - rect.y; }

    for (auto i = rect.x; i < rect.x + rect.w; i++)
    {
        for (auto j = rect.y; j < rect.y + rect.h; j++)
        {
            int32_t index = (j * mWidth) + i;
            auto& cell = mCells.at(index);
            cell.character = character;
        }
    }

    mRerender = true;

    return;
}

void conslr::Screen::setCell(int32_t x, int32_t y, const SDL_Color& background, const SDL_Color& foreground, const int32_t& character)
{
    if (x < 0) { x = 0; }
    if (x >= mWidth) { x = mWidth - 1; }
    if (y < 0) { y = 0; }
    if (y >= mHeight) { y = mHeight - 1; }

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
    if (x < 0) { x = 0; }
    if (x >= mWidth) { x = mWidth - 1; }
    if (y < 0) { y = 0; }
    if (y >= mHeight) { y = mHeight - 1; }

    int32_t index = (y * mWidth) + x;
    auto& cell = mCells.at(index);
    cell.background = background;

    mRerender = true;

    return;
}

void conslr::Screen::setCellForeground(int32_t x, int32_t y, const SDL_Color& foreground)
{
    if (x < 0) { x = 0; }
    if (x >= mWidth) { x = mWidth - 1; }
    if (y < 0) { y = 0; }
    if (y >= mHeight) { y = mHeight - 1; }

    int32_t index = (y * mWidth) + x;
    auto& cell = mCells.at(index);
    cell.foreground = foreground;

    mRerender = true;

    return;
}

void conslr::Screen::setCellForeground(int32_t x, int32_t y, const SDL_Color& foreground, const int32_t& character)
{
    if (x < 0) { x = 0; }
    if (x >= mWidth) { x = mWidth - 1; }
    if (y < 0) { y = 0; }
    if (y >= mHeight) { y = mHeight - 1; }

    int32_t index = (y * mWidth) + x;
    auto& cell = mCells.at(index);
    cell.foreground = foreground;
    cell.character = character;

    mRerender = true;

    return;
}

void conslr::Screen::setCellCharacter(int32_t x, int32_t y, const int32_t& character)
{
    if (x < 0) { x = 0; }
    if (x >= mWidth) { x = mWidth - 1; }
    if (y < 0) { y = 0; }
    if (y >= mHeight) { y = mHeight - 1; }

    int32_t index = (y * mWidth) + x;
    auto& cell = mCells.at(index);
    cell.character = character;

    mRerender = true;

    return;
}

void conslr::Screen::borderRect(SDL_Rect rect, int32_t horizontal, int32_t vertical, int32_t cornerTl, int32_t cornerTr, int32_t cornerBl, int32_t cornerBr)
{
    if (rect.x < 0) { rect.x = 0; }
    if (rect.y < 0) { rect.y = 0; }
    if (rect.x + rect.w > mWidth) { rect.w = mWidth - rect.x; }
    if (rect.y + rect.h > mHeight) { rect.h = mHeight - rect.y; }

    for (auto i = rect.x; i < rect.x + rect.w; i++)
    {
        int32_t index = (rect.y * mWidth) + i;
        int32_t index2 = ((rect.y + rect.h - 1) * mWidth) + i;

        mCells.at(index).character = horizontal;
        mCells.at(index2).character = horizontal;
    }

    for (auto j = rect.y; j < rect.y + rect.h; j++)
    {
        int32_t index = (j * mWidth) + rect.x;
        int32_t index2 = (j * mWidth) + rect.x + rect.w - 1;

        mCells.at(index).character = vertical;
        mCells.at(index2).character = vertical;
    }

    int32_t tl = (rect.y * mWidth) + rect.x;
    int32_t bl = ((rect.y + rect.h - 1) * mWidth) + rect.x;
    mCells.at(tl).character = cornerTl;
    mCells.at(tl + rect.w - 1).character = cornerTr;
    mCells.at(bl).character = cornerBl;
    mCells.at(bl + rect.w - 1).character = cornerBr;

    mRerender = true;

    return;
}

void conslr::Screen::renderText(int32_t x, int32_t y, int32_t maxSize, const std::string& str)
{
    if (x < 0) { x = 0; }
    if (x >= mWidth) { x = mWidth - 1; }
    if (y < 0) { y = 0; }
    if (y >= mHeight) { y = mHeight - 1; }

    int32_t itrSize = std::min(maxSize, (int32_t)str.size());
    int32_t start = (y * mWidth) + x;
    for (int i = 0; i < itrSize; i++)
    {
        mCells.at(start + i).character = (int32_t)str.at(i);
    }

    mRerender = true;

    return;
}

void conslr::Screen::renderMultilineText(int32_t x, int32_t y, int32_t maxWidth, int32_t maxHeight, const std::string& str)
{
    if (x < 0) { x = 0; }
    if (x >= mWidth) { x = mWidth - 1; }
    if (y < 0) { y = 0; }
    if (y >= mHeight) { y = mHeight - 1; }

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

        mCells.at(((j + y) * mWidth) + x + i).character = c;
        i++;
    }

    mRerender = true;

    return;
}

void conslr::Screen::renderTextColor(int32_t x, int32_t y, int32_t maxSize, const std::string& str, const SDL_Color& color)
{
    if (x < 0) { x = 0; }
    if (x >= mWidth) { x = mWidth - 1; }
    if (y < 0) { y = 0; }
    if (y >= mHeight) { y = mHeight - 1; }

    int32_t itrSize = std::min(maxSize, (int32_t)str.size());
    int32_t start = (y * mWidth) + x;
    for (int i = 0; i < itrSize; i++)
    {
        auto& cell = mCells.at(start + i);

        cell.character = (int32_t)str.at(i);
        cell.foreground = color;
    }

    mRerender = true;

    return;
}

void conslr::Screen::renderTextColor(int32_t x, int32_t y, int32_t maxSize, const std::string& str, const SDL_Color& foreground, const SDL_Color& background)
{
    if (x < 0) { x = 0; }
    if (x >= mWidth) { x = mWidth - 1; }
    if (y < 0) { y = 0; }
    if (y >= mHeight) { y = mHeight - 1; }

    int32_t itrSize = std::min(maxSize, (int32_t)str.size());
    int32_t start = (y * mWidth) + x;
    for (int i = 0; i < itrSize; i++)
    {
        auto& cell = mCells.at(start + i);

        cell.character = (int32_t)str.at(i);
        cell.foreground = foreground;
        cell.background = background;
    }

    mRerender = true;

    return;
}

void conslr::Screen::renderMultilineTextColor(int32_t x, int32_t y, int32_t maxWidth, int32_t maxHeight, const std::string& str, const SDL_Color& color)
{
    if (x < 0) { x = 0; }
    if (x >= mWidth) { x = mWidth - 1; }
    if (y < 0) { y = 0; }
    if (y >= mHeight) { y = mHeight - 1; }

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
        auto& cell = mCells.at(((j + y) * mWidth) + x + i);

        cell.character = c;
        cell.foreground = color;

        i++;
    }

    mRerender = true;

    return;
}

void conslr::Screen::renderMultilineTextColor(int32_t x, int32_t y, int32_t maxWidth, int32_t maxHeight, const std::string& str, const SDL_Color& foreground, const SDL_Color& background)
{
    if (x < 0) { x = 0; }
    if (x >= mWidth) { x = mWidth - 1; }
    if (y < 0) { y = 0; }
    if (y >= mHeight) { y = mHeight - 1; }

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

        auto& cell = mCells.at(((j + y) * mWidth) + x + i);

        cell.character = c;
        cell.foreground = foreground;
        cell.background = background;
        i++;
    }

    mRerender = true;

    return;
}

void conslr::Screen::renderTextTagged(int32_t x, int32_t y, int32_t maxSize, const TaggedString& str, const SDL_Color& foreground, const SDL_Color& background)
{
    if (x < 0) { x = 0; }
    if (x >= mWidth) { x = mWidth - 1; }
    if (y < 0) { y = 0; }
    if (y >= mHeight) { y = mHeight - 1; }

    //Gets tagged string vector
    const auto& tstr = str.getString();
    int32_t itrSize = std::min(maxSize, (int32_t)tstr.size());
    int32_t start = (y * mWidth) + x;
    for (int i = 0; i < itrSize; i++)
    {
        auto& cell = mCells.at(start + i);
        //Current tagged character
        const auto& tc = tstr.at(i);
        //Current indexs of the color tags
        int8_t bg = tc.tags & TaggedString::TaggedChar::BACKGROUND_MASK;
        int8_t fg = (tc.tags & TaggedString::TaggedChar::FOREGROUND_MASK) >> 4;

        cell.character = (int32_t)tc.character;

        if (fg == 0)
        {
            cell.foreground = foreground;
        } else
        {
            cell.foreground = str.getTag(fg);
        }

        if (bg == 0)
        {
            cell.background = background;
        } else
        {
            cell.background = str.getTag(bg);
        }
    }

    mRerender = true;

    return;
}

void conslr::Screen::renderMultilineTextTagged(int32_t x, int32_t y, int32_t maxWidth, int32_t maxHeight, const TaggedString& str, const SDL_Color& foreground, const SDL_Color& background)
{
    if (x < 0) { x = 0; }
    if (x >= mWidth) { x = mWidth - 1; }
    if (y < 0) { y = 0; }
    if (y >= mHeight) { y = mHeight - 1; }

    int32_t i = 0;
    int32_t j = 0;
    for (const auto& tc : str.getString())
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

        auto& cell = mCells.at(((j + y) * mWidth) + x + i);
        int8_t bg = tc.tags & TaggedString::TaggedChar::BACKGROUND_MASK;
        int8_t fg = (tc.tags & TaggedString::TaggedChar::FOREGROUND_MASK) >> 4;

        cell.character = (int32_t)tc.character;

        if (fg == 0)
        {
            cell.foreground = foreground;
        } else
        {
            cell.foreground = str.getTag(fg);
        }

        if (bg == 0)
        {
            cell.background = background;
        } else
        {
            cell.background = str.getTag(bg);
        }

        i++;
    }

    mRerender = true;

    return;
}

void conslr::Screen::clear()
{
    mCells.assign(mCells.size(), { mWidgetManager.getTheme()->background, { 255, 255, 255, 255 }, 0 });

    return;
}
