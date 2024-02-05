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
        IWidget(int32_t id, int32_t priority) :
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
        [[nodiscard]] int32_t getId() const { return mId; }
        ///Gets the priority of the widget
        ///
        ///@return Priority of the widget
        [[nodiscard]] int32_t getPriority() const { return mPriority; }
        ///Gets if the widget is active
        ///
        ///@return If the widget is active
        [[nodiscard]] bool getActive() const { return mActive; }

        ///Sets the active status of the widget
        ///Primarily internal
        ///
        ///@param val Value to set it to
        void setActive(bool val) { mActive = val; }

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
        ///Function called by the widget manager
        ///Screen that owns the widget manager
        ///
        virtual void render(Screen&) {}

        ///
        ///Shows the widget
        ///
        void show() { mVisible = true; mRerender = true; }
        ///
        ///Hides the widget
        ///
        void hide() { mVisible = false; mRerender = true; }
        ///Gets the visibility of the widget
        ///
        ///@return True if the widget is visible, false if the widget is not visible
        [[nodiscard]] bool isVisible() const { return mVisible; }

        ///
        ///Shows the widgets title
        ///
        void showTitle() { mShowTitle = true; mRerender = true; }
        ///
        ///Hides the widgets title
        ///
        void hideTitle() { mShowTitle = false; mRerender = true; }

        ///Gets the widgets title
        ///
        ///@return Title of the widget
        [[nodiscard]] const std::string& getTitle() const { return mTitle; }

        ///Sets the widgets title
        ///
        ///@param title Title of the widget
        void setTitle(const std::string& title) { mTitle = title; }
        ///Sets the widgets theme
        ///Should primarily be internally used
        ///Assumes that the theme pointer is always valid
        ///
        ///@param theme Pointer to the theme
        void setTheme(Theme* theme) { mTheme = theme; }

        bool mRerender = true; //!<If the widget is to be rerendered
    protected:
        bool mVisible = true; //!<If the widget is to be rendered
        bool mShowTitle = false; //!<If the widget is to display its title
        std::string mTitle; //!<Title of the widget

        Theme* mTheme = nullptr; //!<Current theme, typically the theme of the console
    };

    ///
    ///Interface for scrollable widgets
    ///
    class IScrollable
    {
    public:
        ///
        ///Scrolls up
        ///
        virtual void scrollUp() {}
        ///
        ///Scrolls down
        ///
        virtual void scrollDown() {}

        ///Gets the index of the currently selected item
        ///
        ///@return Index of the current item
        [[nodiscard]] int32_t getSelection() const { return mSelection; }

    protected:
        int32_t mSelection = 0; //!<Currently selected item
    };

    ///
    ///Interface for widgets that can take text input
    ///
    class ITextInput
    {
    public:
        ///Handles text input
        ///
        ///@param event Text input event
        virtual void doTextInput(SDL_TextInputEvent& event) {}
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
