///
///@file conslr/widgetmanager.hpp
///@brief Contains declarations for WidgetManager
///
///To use widget manager
///1) Create a screen for a console
///2) Get widget manager from screen
///3) Add widgets to screen with createWidget<T>(), where type T inherits from IWidget but is not an interface
#pragma once

#include <cstdint>
#include <memory>
#include <queue>
#include <array>
#include <list>
#include <type_traits>
#include <string>
#include <unordered_map>
#include <stdexcept>

#include "conslr/widget.hpp"

namespace conslr
{
    struct Theme;
    class Screen;

    class WidgetManager
    {
    public:
        friend class Screen;

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
            requires (!std::same_as<T, IWidget>)
        [[nodiscard]] std::weak_ptr<T> createWidget(int32_t priority = 0)
        {
            if (mFreeWidgets.empty())
            {
                throw std::runtime_error("WidgetManager has reached the max number of widgets, max: " + std::to_string(MAX_WIDGETS));
            }

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

        ///Creates widgets from a file
        ///Clears existing widgets to do so
        ///
        ///@param file File name
        ///@return Map of widget names as a string and widget index as the value
        [[nodiscard]] std::unordered_map<std::string, int32_t> loadFromFile(const std::string& file);

        void clear();

        //Getters
        template<IsWidget T>
        [[nodiscard]] constexpr std::weak_ptr<T> getWidget(int index)
        {
            if (!(index >= 0 && index < MAX_WIDGETS))
            {
                throw std::invalid_argument("Index is out of bounds, index: " + std::to_string(index));
            }
            if (mWidgets.at(index) == nullptr)
            {
                throw std::runtime_error("Widget at index is nullptr, index: " + std::to_string(index));
            }

            return std::dynamic_pointer_cast<T>(mWidgets.at(index));
        }
        [[nodiscard]] constexpr const std::list<std::shared_ptr<IRenderable>>& getRenderable() const noexcept { return mRenderable; }
        [[nodiscard]] std::weak_ptr<Theme> getTheme() const noexcept { return mTheme; }

        //Setters
        void setTheme(std::shared_ptr<Theme> theme) noexcept 
        {
            mTheme = theme; 
            
            for (auto& widget : mRenderable)
            {
                widget->setTheme(mTheme);                
            }
        }

        static const int32_t MAX_WIDGETS = 16;

    private:
        WidgetManager();

        std::queue<int32_t> mFreeWidgets;
        std::array<std::shared_ptr<IWidget>, MAX_WIDGETS> mWidgets;

        std::list<std::shared_ptr<IRenderable>> mRenderable;

        std::shared_ptr<Theme> mTheme = nullptr;
    };
}
