#include "conslr/screen.hpp"

#include <SDL_events.h>

conslr::Screen::Screen(int32_t width, int32_t height) :
    mWidth{ width }, mHeight{ height },
    eventCallback{ nullptr }, update{ nullptr },
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

void conslr::Screen::borderRect(SDL_Rect rect, int32_t horizontal, int32_t vertical, int32_t corner)
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
    mCells.at(tl).character = corner;
    mCells.at(tl + rect.w - 1).character = corner;
    mCells.at(bl).character = corner;
    mCells.at(bl + rect.w - 1).character = corner;

    mRerender = true;

    return;
}

constexpr void conslr::Screen::clear()
{
    mCells.assign(mCells.size(), Cell{});

    return;
}
