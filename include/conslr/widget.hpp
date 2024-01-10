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

#include <SDL_rect.h>

namespace conslr
{
    class Screen;
    struct ColorScheme;

    ///
    ///Interface for all widgets
    ///
    class IWidget
    {
    public:
        IWidget(int32_t id, int32_t priority) : mId{ id }, mPriority{ priority } {}
        virtual ~IWidget() {}

        [[nodiscard]] int32_t getId() const { return mId; }
        [[nodiscard]] int32_t getPriority() const { return mPriority; }

    private:
        int32_t mId;
        int32_t mPriority;
    };

    ///
    ///Interface for renderable widgets
    ///
    class IRenderable
    {
    public:
        virtual void render(Screen&) {}

        void show() { mVisible = true; mRerender = true; }
        void hide() { mVisible = false; mRerender = true; }
        [[nodiscard]] bool isVisible() const { return mVisible; }

        void setColorScheme(ColorScheme* colorScheme) { mColorScheme = colorScheme; }

        bool mRerender = true;
    protected:
        bool mVisible = true;

        ColorScheme* mColorScheme = nullptr;
    };

    ///
    ///Interface for scrollable widgets
    ///
    class IScrollable
    {
    public:
        virtual void scrollUp() {}
        virtual void scrollDown() {}

        [[nodiscard]] int32_t getSelection() const { return mSelection; }

    protected:
        int32_t mSelection = 0;
    };

    template <typename T>
    concept IsWidget = std::is_base_of<IWidget, T>::value;

    template <typename T>
    concept IsNotInterface =
        !std::is_same<IWidget, T>::value &&
        !std::is_same<IRenderable, T>::value &&
        !std::is_same<IScrollable, T>::value;
}
