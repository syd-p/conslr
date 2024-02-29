///
///@file conslr/widget.hpp
///@brief Defines widget interfaces
///
///For extending widgets
///1) Inherit from IWidget
///2) Inherit from needed Interfaces
///3) Create a constructor that takes id and priority and passes to IWidget contstructor
///4) Overload interface functions
///5) (For renderables) Make Screen a friend class
///
///Optionally (but encouraged) the constructor and the render method should be marked protected or private and the WidgetManager made a friend class
#pragma once

#include <cstdint>
#include <type_traits>
#include <string>

#include <SDL_rect.h>
#include <SDL_events.h>

namespace conslr
{
    class Screen;
    class WidgetManager;
    struct Theme;

    class IWidget
    {
    public:
        friend class conslr::WidgetManager;
       
        virtual ~IWidget() {}

        //Getters
        [[nodiscard]] constexpr int32_t getId() const noexcept { return mId; }
        [[nodiscard]] constexpr int32_t getPriority() const noexcept { return mPriority; }
        [[nodiscard]] constexpr bool getActive() const noexcept { return mActive; }

        ///Sets the active status of the widget
        ///May be used prior to initial render, as this does not make the screen rerender
        ///Primarily internal
        ///
        ///@param val Value to set it to
        constexpr void setActive(bool val) noexcept { mActive = val; }

    protected:
        constexpr IWidget(int32_t id, int32_t priority) noexcept :
            mId{ id },
            mPriority{ priority },
            mActive{ false }
        {}

        int32_t mId; //!<Internal id of the widget
        int32_t mPriority; //!<Priority of the widget, higher values will be drawn above lower values
        bool mActive; //!<If this widget is currently active, used for changing behavior of widgets
    };

    class IRenderable
    {
    public:
        friend class conslr::Screen;
        friend class conslr::WidgetManager;

        constexpr void show() noexcept { mVisible = true; mRerender = true; }
        constexpr void hide() noexcept { mVisible = false; mRerender = true; }
        [[nodiscard]] constexpr bool isVisible() const noexcept { return mVisible; }

        constexpr void showTitle() noexcept { mShowTitle = true; mRerender = true; }
        constexpr void hideTitle() noexcept { mShowTitle = false; mRerender = true; }

        //Getters
        [[nodiscard]] constexpr const std::string& getTitle() const noexcept { return mTitle; }

        constexpr void setTitle(const std::string& title) { mTitle = title; }
        ///Sets the widgets theme
        ///Should primarily be internally used
        ///Assumes that the theme pointer is always valid
        ///
        ///@param theme Pointer to the theme
        constexpr void setTheme(Theme* theme) noexcept { mTheme = theme; }

        bool mRerender; //!<If the widget is to be rerendered
    protected:
        constexpr IRenderable() noexcept :
            mRerender{ true }, mVisible{ true },
            mShowTitle{ false },
            mTheme{ nullptr }
        {}

        virtual void render(Screen&) {}

        bool mVisible; //!<If the widget is to be rendered
        bool mShowTitle; 
        std::string mTitle; 

        Theme* mTheme; //!<Current theme, typically the theme of the console
    };

    class IScrollable
    {
    public:
        friend class conslr::WidgetManager;

        virtual constexpr void scrollUp() noexcept {}
        virtual constexpr void scrollDown() noexcept {}

        [[nodiscard]] constexpr int32_t getSelection() const noexcept { return mSelection; }

    protected:
        constexpr IScrollable() noexcept :
            mSelection{ 0 }
        {}

        int32_t mSelection; //!<Currently selected item
    };

    class ITextInput
    {
    public:
        friend class conslr::WidgetManager;

        virtual void doTextInput(SDL_TextInputEvent& event) noexcept {}

    protected:
        constexpr ITextInput() noexcept {}
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
