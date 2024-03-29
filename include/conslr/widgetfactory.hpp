///
///@file conslr/widgetfactory.hpp
///@brief Definition for the WidgetFactory used to load from files
///
#pragma once

#include <stdexcept>
#include <unordered_map>
#include <string>
#include <functional>

#include "conslr/widgetmanager.hpp"
#include "conslr/widget.hpp"

namespace conslr
{
    typedef std::unordered_map<std::string, std::string> WidgetParameterMap;
    typedef std::function<std::pair<std::string, int32_t>(WidgetManager&, const WidgetParameterMap&)> WidgetFactorySignature;

    class WidgetFactory
    {
    public:
        WidgetFactory() = delete;

        static void initialize();

        static void registerWidget(const std::string& key, WidgetFactorySignature factory)
        {
            if (mFactories.contains(key))
            {
                throw std::runtime_error("Key already registered, key: " + key);
            }

            mFactories.insert({ key, factory });

            return;
        }

        [[nodiscard]] static std::pair<std::string, int32_t> createWidget(const std::string& key, WidgetManager& wm, const WidgetParameterMap& params)
        {
            if (!mFactories.contains(key))
            {
                throw std::invalid_argument("Key not registered with WidgetFactory, key: " + key);
            }

            return mFactories.at(key)(wm, params);
        }

    private:
        static std::unordered_map<std::string, WidgetFactorySignature> mFactories;
        static bool mInitialized;
    };
}
