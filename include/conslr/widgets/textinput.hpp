///
///@file conslr/widgets/textinput.hpp
///@brief Defines the TextInput widget
///
#pragma once

#include <string>
#include <vector>

#include <SDL_rect.h>

#include "conslr/widget.hpp"
#include "conslr/screen.hpp"
#include "conslr/theme.hpp"

namespace conslr::widgets
{
    ///Renders a simple text box
    ///
    ///Only newline characters are treated as a special character
    ///Other characters such as \t are treated as a index for a character to render
    ///Note: Scrollbar may be inaccurate for certain amounts of rows, not terribly inaccurate so it has been left to be fixed later
    class TextInput : public IWidget, public IRenderable, public ITextInput
    {
    public:
        ///
        ///Internal constructor
        ///
        TextInput(int32_t id, int32_t priority) :
            IWidget{ id, priority },
            mRegion{ 0, 0, 0, 0 }, mTextRegion{ 0, 0, 0, 0, },
            mMaxRows{ -1 },
            mCurrentRow{ 0 }, mSelection{ 0 },
            mWordWrap{ false },
            mScrollX{ 0 }, mScrollY{ 0 }
        {
            mRows.emplace_back();

            return;
        }

        ///
        ///Handles text input
        ///
        virtual void doTextInput(SDL_TextInputEvent& event) override
        {
            if (!mActive)
            {
                return;
            }

            mRows.at(mCurrentRow).insert(mSelection, event.text);
            mSelection += strlen(event.text);

            mRerender = true;

            return;
        }

        ///
        ///Move selection left
        ///
        virtual void doKeyLeft()
        {
            if (!mActive) { return; }
            mSelection = std::max(0, mSelection - 1);
            mRerender = true;
            return;
        }
        ///
        ///Move selection right
        ///
        virtual void doKeyRight()
        {
            if (!mActive) { return; }
            mSelection = std::min((int32_t)mRows.at(mCurrentRow).size(), mSelection + 1);
            mRerender = true;
            return;
        }
        ///
        ///Move selection up
        ///
        virtual void doKeyUp()
        {
            if (!mActive) { return; }
            mCurrentRow = std::max(0, mCurrentRow - 1);
            mSelection = std::min((int32_t)mRows.at(mCurrentRow).size(), mSelection);
            mRerender = true;
            return;
        }
        ///
        ///Move selection down
        ///
        virtual void doKeyDown()
        {
            if (!mActive) { return; }
            mCurrentRow = std::min((int32_t)mRows.size() - 1, mCurrentRow + 1);
            mSelection = std::min((int32_t)mRows.at(mCurrentRow).size(), mSelection);
            mRerender = true;
            return;
        }
        ///
        ///Tab key
        ///
        virtual void doTab()
        {
            if (!mActive) { return; }
            mRows.at(mCurrentRow).insert(mSelection, "    ");
            mSelection += 4;
            mRerender = true;
            return;
        }
        ///
        ///Backspace key
        ///
        virtual void doBackspace()
        {
            if (!mActive) { return; }
            if (mSelection != 0)
            {
                mRows.at(mCurrentRow).erase(mSelection - 1, 1);
                mSelection--;
                mRerender = true;
                return;
            }
            if (mCurrentRow != 0)
            {
                mSelection = mRows.at(mCurrentRow - 1).size();
                mRows.at(mCurrentRow - 1).append(mRows.at(mCurrentRow));
                mRows.erase(mRows.begin() + mCurrentRow);
                mCurrentRow--;
                mRerender = true;
                return;
            }

            return;
        }
        ///
        ///Return key
        ///
        virtual void doReturn()
        {
            if (!mActive) { return; }
            if (mRows.size() + 1 > mMaxRows) { return; }
            mRows.insert(mRows.begin() + mCurrentRow + 1, std::string{ mRows.at(mCurrentRow).begin() + mSelection, mRows.at(mCurrentRow).end() });
            mRows.at(mCurrentRow).erase(mSelection);

            mCurrentRow++;
            mSelection = 0;
            mRerender = true;
            return;
        }

        ///
        ///Internal
        ///
        virtual void render(Screen& screen) override
        {
            assert((mRegion.w > 2 && mRegion.h > 2) && "Scroll List is too small to render");

            screen.fillRect(mRegion, mTheme->background, mTheme->border, 0);
            screen.borderRect(mRegion, mTheme->borderHorizontal, mTheme->borderVertical, mTheme->borderCornerTl, mTheme->borderCornerTr, mTheme->borderCornerBl, mTheme->borderCornerBr);

            if (mShowTitle)
            {
                screen.renderTextColor(
                        mRegion.x + 1, mRegion.y,
                        std::min(mRegion.w - 2, (int32_t)mTitle.size()),
                        mTitle,
                        mTheme->border
                        );
            }

            if (mWordWrap)
            {
                //Word wrap rendering
            } else
            {
                //No word wrap rendering
                //Cursor
                int32_t cursorX = mTextRegion.x + (mSelection - mScrollX);
                int32_t cursorY = mTextRegion.y + (mCurrentRow - mScrollY);

                //Ensures that cursorX is within the text region
                while (cursorX > mTextRegion.x + mTextRegion.w - 1)
                {
                    mScrollX++;
                    cursorX--;
                }

                while (cursorX < mTextRegion.x)
                {
                    mScrollX--;
                    cursorX++;
                }

                //Ensures that cursorY is within the text region
                while (cursorY > mTextRegion.y + mTextRegion.h - 1)
                {
                    mScrollY++;
                    cursorY--;
                }

                while (cursorY < mTextRegion.y)
                {
                    mScrollY--;
                    cursorY++;
                }

                for (auto i = 0; i < mTextRegion.h; i++)
                {
                    if (i >= mRows.size())
                    {
                        break;
                    }

                    const std::string& row = mRows.at(i + mScrollY);
                    if (row.size() <= mScrollX)
                    {
                        continue;
                    }

                    screen.renderText(mTextRegion.x, mTextRegion.y + i, mTextRegion.w, row.substr(mScrollX));
                }

                screen.setCellBackground(cursorX, cursorY, mTheme->selection);

                //Scrollbar
                if (mRows.size() > mTextRegion.h)
                {
                    //Y scrollbar
                    int32_t visiblePercent = (mTextRegion.h * 100) / mRows.size();
                    int32_t percentDown = (mScrollY * 100) / mRows.size();

                    int32_t scrollbarOffset = (percentDown * mTextRegion.h) / 100;
                    int32_t scrollbarHeight = (visiblePercent * mTextRegion.h) / 100;

                    screen.fillRectCharacter({
                            mRegion.x,
                            mTextRegion.y + scrollbarOffset,
                            1,
                            std::min(scrollbarHeight + 1, mTextRegion.h - scrollbarOffset)
                            }, mTheme->scrollbarCharacter);
                }

                //Finds size of longest string
                int32_t maxWidth = std::max_element(mRows.begin(), mRows.end(),
                        [](const std::string& str1, const std::string& str2)
                        {
                            return str1.size() < str2.size();
                        })->size();

                if (maxWidth > mTextRegion.w)
                {
                    //X scrollbar
                    int32_t visiblePercent = (mTextRegion.w * 100) / maxWidth;
                    int32_t percentOver = (mScrollX * 100) / maxWidth;

                    int32_t scrollbarOffset = (percentOver * mTextRegion.w) / 100;
                    int32_t scrollbarWidth = (visiblePercent * mTextRegion.w) / 100;

                    screen.fillRectCharacter({
                            mTextRegion.x + scrollbarOffset,
                            mRegion.y + mRegion.h - 1,
                            std::min(scrollbarWidth + 1, mTextRegion.w - scrollbarOffset),
                            1
                            }, mTheme->scrollbarCharacterHorizontal);
                }
            }

            return;
        }

        ///Gets the current text
        ///
        ///@return Current text string
        std::string getText() const
        {
            std::string str;
            for (const auto& row : mRows)
            {
                str.append(row);
                str += '\n';
            }

            //Removes last newline char
            str.pop_back();

            return str;
        }
        ///Gets the value of word wrap
        ///
        ///@return Current value of word wrap
        bool getWordWrap() const { return mWordWrap; }
        ///Gets the max rows of the widget
        ///
        ///@return Max rows of the widget, negative is infinite
        int32_t getMaxRows() const { return mMaxRows; }

        ///Sets the region of the widget
        ///
        ///@param region Region of the widget on screen
        void setRegion(const SDL_Rect& region)
        {
            assert(((region.w > 2) && (region.h > 2)) && "Region is too small");

            mRegion = region;
            mTextRegion = { mRegion.x + 1, mRegion.y + 1, mRegion.w - 2, mRegion.h - 2 };

            mRerender = true;
            return;
        }
        ///Sets the text of the widget
        ///
        ///@param text Text of the widget
        void setText(const std::string& text)
        {
            mRows.clear();
            mCurrentRow = 0;
            mSelection = 0;

            std::string str;
            for (const auto& c : text)
            {
                if (c == '\n')
                {
                    mRows.emplace_back(str);
                    str.clear();
                    continue;
                }

                str += c;
            }
            mRows.emplace_back(str);

            mRerender = true;
            return;
        }
        ///Sets the value of word wrap
        ///
        ///@param val Value to set it to
        void setWordWrap(bool val) { mWordWrap = true; mRerender = true; }
        ///Sets the max rows of the widget
        ///Excess rows are deleted
        ///
        ///@param max New max value
        void setMaxRows(int32_t max)
        {
            mMaxRows = max;
            if (mMaxRows == -1)
            {
                return;
            }

            if (mMaxRows < mRows.size())
            {
                //Removes excess rows
                mRows = { mRows.begin(), mRows.begin() + mMaxRows };
            }

            mRerender = true;
            return;
        }

    protected:
        SDL_Rect mRegion; //!<Region of the widget on the screen
        SDL_Rect mTextRegion; //!<Region that text can be rendered in
        std::vector<std::string> mRows; //!<Text of the widget
        int32_t mMaxRows; //!<Max rows for the input, negative is infinite
        int32_t mCurrentRow; //!<Currently selected row
        int32_t mSelection; //!<Index of current selection on row
        bool mWordWrap; //!<If word wrap is enabled
        int32_t mScrollX; //!<X scroll of the text
        int32_t mScrollY; //!<Y scroll of the text
    };
}
