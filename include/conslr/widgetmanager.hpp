///
///@file conslr/widgetmanager.hpp
///@brief Contains declarations for WidgetManager
///
///To use widget manager
///1) Create a screen for a console
///2) Get widget manager from screen
///3) Add widgets to screen with createWidget(...), where type T inherits from IWidget but is not an interface
#pragma once

#include <cstdint>
#include <memory>
#include <queue>
#include <array>
#include <list>
#include <type_traits>
#include <cassert>

#include "conslr/widget.hpp"

namespace conslr
{
    struct Theme;

    class WidgetManager
    {
    public:
        WidgetManager();
        WidgetManager(const WidgetManager&) = delete;
        WidgetManager(WidgetManager&&) = delete;
        WidgetManager& operator=(const WidgetManager&) = delete;
        WidgetManager& operator=(WidgetManager&&) = delete;

        ///Creates a widget
        ///
        ///@tparam T Type of widget to create, type must inherit from IWidget and must not be a widget interface
        ///@param priority Priority of the created widget
        ///@return Returns a weak pointer to the created widget
        template<IsWidget T>
            requires IsNotInterface<T>
        [[nodiscard]] std::weak_ptr<T> createWidget(int32_t priority = 0)
        {
           assert(!mFreeWidgets.empty() && "Max number of widgets created");

            int32_t index = mFreeWidgets.front();
            mFreeWidgets.pop();

            std::shared_ptr<T> ptr{ new T(index, priority) };
            mWidgets.at(index) = ptr;

            if (std::is_base_of<IRenderable, T>())
            {
                ptr->setTheme(mTheme);
                mRenderable.push_back(ptr);
                mRenderable.sort([](const std::shared_ptr<IRenderable>& a, const std::shared_ptr<IRenderable>& b) -> bool
                        {
                            return std::dynamic_pointer_cast<IWidget>(a)->getPriority() < std::dynamic_pointer_cast<IWidget>(b)->getPriority();
                        });
            }

            return ptr;
        }
        void destroyWidget(int32_t index);

        void activateWidget(int32_t index);
        void deactivateWidget(int32_t index);

        void loadFromFile(const std::string& file);

        void clear();

        //Getters
        template<IsWidget T>
        constexpr std::shared_ptr<T> getWidget(int index)
        {
            assert((index >= 0 && index < MAX_WIDGETS) && "Index out of bounds");
            assert((mWidgets.at(index) != nullptr) && "Widget does not exist");

            return std::dynamic_pointer_cast<T>(mWidgets.at(index));
        }
        constexpr const std::list<std::shared_ptr<IRenderable>>& getRenderable() const noexcept { return mRenderable; }
        constexpr const Theme* getTheme() const noexcept { return mTheme; }

        //Setters
        ///Sets the Color Scheme for widgets
        ///Note that the colorscheme passed must have a lifetime at least equal to the life time of the screen that the widget manager is held by
        ///
        ///@param theme Pointer to the color scheme
        constexpr void setTheme(Theme* theme) noexcept { mTheme = theme; }

        static const int32_t MAX_WIDGETS = 16;

    private:
        std::queue<int32_t> mFreeWidgets;
        std::array<std::shared_ptr<IWidget>, MAX_WIDGETS> mWidgets;

        std::list<std::shared_ptr<IRenderable>> mRenderable;

        Theme* mTheme = nullptr;
    };
}
