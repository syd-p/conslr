#pragma once

#include <string>
#include <functional>
#include <memory>
#include <unordered_map>
#include <list>

#include "conslr/widget.hpp"

namespace conslr
{
    typedef std::function<std::shared_ptr<IWidget>(std::list<std::shared_ptr<IRenderable>>&, int32_t, int32_t)> WidgetFactorySignature;

    class WidgetManager;

    ///
    ///Factory class used to construct widgets from a template file
    ///
    class WidgetFactory
    {
    public:
        friend class WidgetManager;

        WidgetFactory() = delete;

        //Registers a widget to be used by WidgetFactory
        template <IsWidget T>
            requires IsNotInterface<T>
        static void registerWidget(const std::string& str)
        {
            mFactories.insert({ str, createWidgetFunc<T> }); 

            return;
        }

    private:
        //Constructs a widget
        [[nodiscard]] static std::shared_ptr<IWidget> createWidget(const std::string& id, std::list<std::shared_ptr<IRenderable>>& renderables, int32_t index, int32_t priority)
        {
            return mFactories.at(id)(renderables, index, priority);
        }

        //Generic function for creating widgets similar to WidgetManager::createWidget
        template <IsWidget T>
            requires IsNotInterface<T>
        static std::shared_ptr<IWidget> createWidgetFunc(std::list<std::shared_ptr<IRenderable>>& renderables, int32_t index, int32_t priority = 0)
        {
            std::shared_ptr<T> ptr{ new T(index, priority) };

            if (std::is_base_of<IRenderable, T>())
            {
                renderables.push_back(ptr);
                renderables.sort([](const std::shared_ptr<IRenderable>& a, const std::shared_ptr<IRenderable>& b) -> bool
                        {
                            return std::dynamic_pointer_cast<IWidget>(a)->getPriority() < std::dynamic_pointer_cast<IWidget>(b)->getPriority();
                        });
            }

            return ptr;
        }

        static std::unordered_map<std::string, WidgetFactorySignature> mFactories;
    };
}
