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
///6) Optionally create a constructor function used by WidgetFactory for loading the widget from a json file
///
///Optionally (but encouraged) the constructor and the render method should be marked protected or private and the WidgetManager made a friend class
#pragma once

#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include <string>
#include <memory>
#include <vector>

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

        ///Can be used before main loop begins
        ///
        ///Otherwise use WidgetManager::activateWidget/deactivateWidget so the screen is rerendered
        constexpr void setActive(bool val) noexcept { mActive = val; }

    protected:
        constexpr IWidget(int32_t id, int32_t priority) noexcept :
            mId{ id },
            mPriority{ priority },
            mActive{ false }
        {}

        int32_t mId;
        int32_t mPriority;
        bool mActive;
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

        //Setters
        constexpr void setTitle(const std::string& title) { mTitle = title; }
        void setTheme(std::shared_ptr<Theme> theme) noexcept { mTheme = theme; }

        bool mRerender;
    protected:
        constexpr IRenderable() noexcept :
            mRerender{ true }, mVisible{ true },
            mShowTitle{ false },
            mTheme{ nullptr }
        {}

        virtual void render(Screen&) {}

        bool mVisible;
        bool mShowTitle; 
        std::string mTitle; 

        std::shared_ptr<Theme> mTheme;
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

        int32_t mSelection;
    };

    class ITextInput
    {
    public:
        friend class conslr::WidgetManager;

        virtual void doTextInput(SDL_TextInputEvent& event) noexcept { (void)event; }

    protected:
        constexpr ITextInput() noexcept {}
    };

    template <typename T>
    struct ListContainer
    {
        size_t mIndex;
        T mElement;
        std::string mName;
    };


    template <typename T>
    class IList
    {
    public:
        friend class conslr::WidgetManager;


        virtual void addElement(const T& element, const std::string& name)
        {
            mElements.emplace_back(mElements.size(), element, name);

            return;
        }

        virtual void removeElement(size_t index)
        {
            if (index < 0 || index >= mElements.size())
            {
                throw std::runtime_error(std::string("Index is out of bounds, index: ") + std::to_string(index) + ", size: " + std::to_string(mElements.size()));
            }

            mElements.erase(mElements.begin() + index);

            for (size_t i = index; i < mElements.size(); i++)
            {
                mElements.at(i).mIndex = i;
            }

            return;
        }

        //Getters
        const ListContainer<T>& getElement(size_t index) { return mElements.at(index); }
        constexpr const std::vector<ListContainer<T>>& getElements() noexcept { return mElements; }
        constexpr size_t size() const noexcept { return mElements.size(); }
        
        //Setters
        void setElementName(size_t index, const std::string& name) { mElements.at(index).mName = name; }
        void setElement(size_t index, const T& element) { mElements.at(index).mElement = element; }

    protected:
        constexpr IList() noexcept {}

        std::vector<ListContainer<T>> mElements;
    };

    template <class T>
    concept IsWidget = std::is_base_of<IWidget, T>::value;

    template <class T>
    concept IsNotInterface =
        !std::is_same<IWidget, T>::value &&
        !std::is_same<IRenderable, T>::value &&
        !std::is_same<IScrollable, T>::value &&
        !std::is_same<ITextInput, T>::value;
}
