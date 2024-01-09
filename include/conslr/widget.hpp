///
///@file conslr/widget.hpp
///@brief Defines widget interfaces
///
#pragma once

#include <cstdint>
#include <type_traits>

#include <SDL_rect.h>

namespace conslr
{
    class Screen;

    ///
    ///Interface for all widgets
    ///
    class IWidget
    {
    public:
        virtual ~IWidget() {}

        int32_t mId; //!<Id of the widget in the WidgetManager
    };

    ///
    ///Interface for renderable widgets
    ///
    class IRenderable
    {
    public:
        virtual void render(Screen&) {}
    };

    template <typename T>
    concept IsWidget = std::is_base_of<IWidget, T>::value;

    template <typename T>
    concept IsNotInterface =
        !std::is_same<IWidget, T>::value &&
        !std::is_same<IRenderable, T>::value;
}
