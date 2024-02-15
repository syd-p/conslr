///
///@file conslr/widget.hpp
///@brief Defines widget interfaces
///
///For extending widgets
///1) Inherit from IWidget
///2) Inherit from needed Interfaces
///3) Create a constructor that takes id and priority and passes to IWidget contstructor
///4) Overload interface functions
#pragma once

#include <cstdint>
#include <type_traits>
#include <string>

#include <SDL_rect.h>
#include <SDL_events.h>

namespace conslr
{
    class Screen;
    struct Theme;

    ///
    ///Interface for all widgets
    ///
    class IWidget
    {
    public:
        ///
        ///Internal constructor
        ///
        constexpr IWidget(int32_t id, int32_t priority) noexcept :
            mId{ id },
            mPriority{ priority },
            mActive{ false }
        {}
        ///
        ///Internal destructor
        ///
        virtual ~IWidget() {}

        ///Gets the id of the widget
        ///
        ///@return Id of the widget
        [[nodiscard]] constexpr int32_t getId() const noexcept { return mId; }
        ///Gets the priority of the widget
        ///
        ///@return Priority of the widget
        [[nodiscard]] constexpr int32_t getPriority() const noexcept { return mPriority; }
        ///Gets if the widget is active
        ///
        ///@return If the widget is active
        [[nodiscard]] constexpr bool getActive() const noexcept { return mActive; }

        ///Sets the active status of the widget
        ///Primarily internal
        ///
        ///@param val Value to set it to
        constexpr void setActive(bool val) noexcept { mActive = val; }

    protected:
        int32_t mId; //!<Internal id of the widget
        int32_t mPriority; //!<Priority of the widget, higher values will be drawn above lower values
        bool mActive; //!<If this widget is currently active, used for changing behavior of widgets
    };

    ///
    ///Interface for renderable widgets
    ///
    class IRenderable
    {
    public:
        ///
        ///Default constructor
        ///
        constexpr IRenderable() noexcept :
            mRerender{ true }, mVisible{ true },
            mShowTitle{ false },
            mTheme{ nullptr }
        {}

        ///
        ///Function called by the widget manager
        ///Screen that owns the widget manager
        ///
        virtual void render(Screen&) {}

        ///
        ///Shows the widget
        ///
        constexpr void show() noexcept { mVisible = true; mRerender = true; }
        ///
        ///Hides the widget
        ///
        constexpr void hide() noexcept { mVisible = false; mRerender = true; }
        ///Gets the visibility of the widget
        ///
        ///@return True if the widget is visible, false if the widget is not visible
        [[nodiscard]] constexpr bool isVisible() const noexcept { return mVisible; }

        ///
        ///Shows the widgets title
        ///
        constexpr void showTitle() noexcept { mShowTitle = true; mRerender = true; }
        ///
        ///Hides the widgets title
        ///
        constexpr void hideTitle() noexcept { mShowTitle = false; mRerender = true; }

        ///Gets the widgets title
        ///
        ///@return Title of the widget
        [[nodiscard]] constexpr const std::string& getTitle() const noexcept { return mTitle; }

        ///Sets the widgets title
        ///
        ///@param title Title of the widget
        constexpr void setTitle(const std::string& title) { mTitle = title; }
        ///Sets the widgets theme
        ///Should primarily be internally used
        ///Assumes that the theme pointer is always valid
        ///
        ///@param theme Pointer to the theme
        constexpr void setTheme(Theme* theme) noexcept { mTheme = theme; }

        bool mRerender; //!<If the widget is to be rerendered
    protected:
        bool mVisible; //!<If the widget is to be rendered
        bool mShowTitle; //!<If the widget is to display its title
        std::string mTitle; //!<Title of the widget

        Theme* mTheme; //!<Current theme, typically the theme of the console
    };

    ///
    ///Interface for scrollable widgets
    ///
    class IScrollable
    {
    public:
        ///
        ///Default constructor
        ///
        constexpr IScrollable() noexcept :
            mSelection{ 0 }
        {}

        ///
        ///Scrolls up
        ///
        virtual constexpr void scrollUp() noexcept {}
        ///
        ///Scrolls down
        ///
        virtual constexpr void scrollDown() noexcept {}

        ///Gets the index of the currently selected item
        ///
        ///@return Index of the current item
        [[nodiscard]] constexpr int32_t getSelection() const noexcept { return mSelection; }

    protected:
        int32_t mSelection; //!<Currently selected item
    };

    ///
    ///Interface for widgets that can take text input
    ///
    class ITextInput
    {
    public:
        ///
        ///Default constructor
        ///
        constexpr ITextInput() noexcept {}

        ///Handles text input
        ///
        ///@param event Text input event
        virtual void doTextInput(SDL_TextInputEvent& event) noexcept {}
    };

    template <typename T>
    concept IsWidget = std::is_base_of<IWidget, T>::value;

    template <typename T>
    concept IsNotInterface =
        !std::is_same<IWidget, T>::value &&
        !std::is_same<IRenderable, T>::value &&
        !std::is_same<IScrollable, T>::value &&
        !std::is_same<ITextInput, T>::value;
}
