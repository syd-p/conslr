///
///@file conslr/widgetmanager.hpp
///@brief Contains declarations for WidgetManager
///
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
    ///
    ///WidgetManager class
    ///
    class WidgetManager
    {
    public:
        WidgetManager();

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

            std::shared_ptr<T> ptr = std::make_shared<T>(index, priority);
            mWidgets.at(index) = ptr;

            if (std::is_base_of<IRenderable, T>())
            {
                mRenderable.push_back(ptr);
                mRenderable.sort([](const std::shared_ptr<IRenderable>& a, const std::shared_ptr<IRenderable>& b) -> bool
                        {
                            return std::dynamic_pointer_cast<IWidget>(a)->getPriority() < std::dynamic_pointer_cast<IWidget>(b)->getPriority();
                        });
            }

            return ptr;
        }
        ///Destroys a widget
        ///
        ///@param index Index of widget to destroy
        void destroyWidget(int32_t index);

        //Getters
        ///Gets a widget as a shared pointer
        ///
        ///@tparam T Type to return the widget as
        ///@param index Index of the widget
        ///@return Shared pointer to the widget as type T
        template<IsWidget T>
        std::shared_ptr<T> getWidget(int index)
        {
            assert((index >= 0 && index < MAX_WIDGETS) && "Index out of bounds");
            assert((mWidgets.at(index) != nullptr) && "Widget does not exist");

            return std::dynamic_pointer_cast<T>(mWidgets.at(index));
        }
        ///Returns the list of renderable widgets
        ///
        ///@return A const reference to the list of renderable widgets
        const std::list<std::shared_ptr<IRenderable>>& getRenderable() const { return mRenderable; }

        static const int32_t MAX_WIDGETS = 16; //!<Max widgets that a WidgetManager can hold

    private:
        std::queue<int32_t> mFreeWidgets;
        std::array<std::shared_ptr<IWidget>, MAX_WIDGETS> mWidgets;

        std::list<std::shared_ptr<IRenderable>> mRenderable;
    };
}