///
///@file conslr/widgets/textinput.hpp
///@brief Defines the TextInput widget
///
#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

#include <SDL_rect.h>

#include "conslr/widget.hpp"
#include "conslr/screen.hpp"
#include "conslr/theme.hpp"
#include "conslr/widgetmanager.hpp"
#include "conslr/widgetfactory.hpp"

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
        friend class conslr::Screen;
        friend class conslr::WidgetManager;

        virtual void doTextInput(SDL_TextInputEvent& event) noexcept override
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

        virtual constexpr void doKeyLeft() noexcept
        {
            if (!mActive) { return; }
            mSelection = std::max(0, mSelection - 1);
            mRerender = true;
            return;
        }
        virtual constexpr void doKeyRight() noexcept
        {
            if (!mActive) { return; }
            mSelection = std::min((int32_t)mRows.at(mCurrentRow).size(), mSelection + 1);
            mRerender = true;
            return;
        }
        virtual constexpr void doKeyUp() noexcept
        {
            if (!mActive) { return; }
            mCurrentRow = std::max(0, mCurrentRow - 1);
            mSelection = std::min((int32_t)mRows.at(mCurrentRow).size(), mSelection);
            mRerender = true;
            return;
        }
        virtual constexpr void doKeyDown() noexcept
        {
            if (!mActive) { return; }
            mCurrentRow = std::min((int32_t)mRows.size() - 1, mCurrentRow + 1);
            mSelection = std::min((int32_t)mRows.at(mCurrentRow).size(), mSelection);
            mRerender = true;
            return;
        }
        virtual constexpr void doTab()
        {
            if (!mActive) { return; }
            mRows.at(mCurrentRow).insert(mSelection, "    ");
            mSelection += 4;
            mRerender = true;
            return;
        }
        virtual constexpr void doBackspace()
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
        virtual constexpr void doReturn()
        {
            if (!mActive) { return; }
            if (mRows.size() + 1 > (size_t)mMaxRows) { return; }
            mRows.insert(mRows.begin() + mCurrentRow + 1, std::string{ mRows.at(mCurrentRow).begin() + mSelection, mRows.at(mCurrentRow).end() });
            mRows.at(mCurrentRow).erase(mSelection);

            mCurrentRow++;
            mSelection = 0;
            mRerender = true;
            return;
        }

        //Getters
        constexpr const SDL_Rect& getRegion() const noexcept { return mRegion; }
        constexpr std::string getString() const
        {
            std::string str;
            for (const auto& row : mRows)
            {
                str.append(row);
                str += '\n';
            }

            //Removes last newline char
            if (str.size() > 0)
            {
                str.pop_back();
            }

            return str;
        }
        constexpr bool getWordWrap() const noexcept { return mWordWrap; }
        constexpr int32_t getMaxRows() const noexcept { return mMaxRows; }

        //Setters
        constexpr void setRegion(const SDL_Rect& region) noexcept
        {
            assert(((region.w > 2) && (region.h > 2)) && "Region is too small");

            mRegion = region;
            mTextRegion = { mRegion.x + 1, mRegion.y + 1, mRegion.w - 2, mRegion.h - 2 };

            mRerender = true;
            return;
        }
        constexpr void setString(const std::string& text)
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
        constexpr void setWordWrap(bool val) noexcept { mWordWrap = val; mRerender = true; }
        constexpr void setMaxRows(int32_t max)
        {
            mMaxRows = max;
            if (mMaxRows == -1)
            {
                return;
            }

            if ((size_t)mMaxRows < mRows.size())
            {
                //Removes excess rows
                mRows = { mRows.begin(), mRows.begin() + mMaxRows };
            }

            mRerender = true;
            return;
        }

    protected:
        constexpr TextInput(int32_t id, int32_t priority) noexcept :
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
                    if ((size_t)i >= mRows.size())
                    {
                        break;
                    }

                    const std::string& row = mRows.at(i + mScrollY);
                    if (row.size() <= (size_t)mScrollX)
                    {
                        continue;
                    }

                    screen.renderText(mTextRegion.x, mTextRegion.y + i, mTextRegion.w, row.substr(mScrollX));
                }

                if (mActive)
                {
                    screen.setCellBackground(cursorX, cursorY, mTheme->selection);
                }

                //Scrollbar
                if (mRows.size() > (size_t)mTextRegion.h)
                {
                    double visiblePercent = (double)mTextRegion.h / (double)mRows.size();
                    double percentDown = (double)mScrollY / (double)mRows.size();

                    int32_t scrollbarOffset = percentDown * mTextRegion.h;
                    int32_t scrollbarHeight = visiblePercent * mTextRegion.h;

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
                    double visiblePercent = (double)mTextRegion.w / (double)maxWidth;
                    double percentOver = (double)mScrollX / (double)maxWidth;

                    int32_t scrollbarOffset = percentOver * mTextRegion.w;
                    int32_t scrollbarWidth = visiblePercent * mTextRegion.w;

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

        SDL_Rect mRegion;
        SDL_Rect mTextRegion;
        std::vector<std::string> mRows;
        int32_t mMaxRows; //!<Negative values indicate infinite rows
        int32_t mCurrentRow;
        int32_t mSelection;
        bool mWordWrap;
        int32_t mScrollX;
        int32_t mScrollY;
    };

    inline std::pair<std::string, int32_t> constructTextInput(WidgetManager& wm, const WidgetParameterMap& params)
    {
        int priority = 0;
        if (params.contains("priority"))
        {
            priority = std::stoi(params.at("priority"));
        }
        auto wptr = wm.createWidget<TextInput>(priority);
        auto ptr = wptr.lock();

        if (params.contains("visible"))
        {
            if (params.at("visible") == "true")
            {
                ptr->show();
            } 
            else if (params.at("visible") == "false")
            {
                ptr->hide();
            }
            else
            {
                throw std::invalid_argument("Param visible must be \"true\" or \"false\"");
            }
        }

        if (params.contains("showtitle"))
        {
            if (params.at("showtitle") == "true")
            {
                ptr->showTitle();
            }
            else if (params.at("showtitle") == "false")
            {
                ptr->hideTitle();
            } 
            else
            {
                throw std::invalid_argument("Param showtitle must be \"true\" or \"false\"");
            }
        }

        if (params.contains("active"))
        {
            if (params.at("active") == "true")
            {
                ptr->setActive(true);
            }
            else if (params.at("active") == "false")
            {
                ptr->setActive(false);
            } 
            else
            {
                throw std::invalid_argument("Param active must be \"true\" or \"false\"");
            }
        }

        if (params.contains("string"))
        {
            ptr->setString(params.at("string"));
        }

        if (params.contains("title"))
        {
            ptr->setTitle(params.at("title"));
        }

        if (params.contains("region"))
        {
            std::stringstream ss{ params.at("region") };
            int32_t x;
            int32_t y;
            int32_t w;
            int32_t h;

            ss >> x >> y >> w >> h;
            if (ss.fail())
            {
                throw std::invalid_argument("Param region must be in the format of \"intx inty intw inth\"");
            }

            ptr->setRegion({ x, y, w, h });
        }

        if (params.contains("maxrows"))
        {
            ptr->setMaxRows(std::stoi(params.at("maxrows")));
        }

        if (params.contains("name"))
        {
            return { params.at("name"), ptr->getId() };
        }
        else
        {
            return { "unnamed", ptr->getId() };
        }
    } 
}
