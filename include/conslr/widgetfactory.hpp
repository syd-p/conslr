///
///@file conslr/widgetfactory.hpp
///@brief Definition for the WidgetFactory used to load from files
///
#pragma once

#include <unordered_map>
#include <string>
#include <functional>

#include "conslr/widgetmanager.hpp"
#include "conslr/widget.hpp"

namespace conslr
{
    typedef std::unordered_map<std::string, std::string> WidgetParameterMap;
    typedef std::function<void(WidgetManager&, const WidgetParameterMap&)> WidgetFactorySignature;

    class WidgetFactory
    {
    public:
        WidgetFactory() = delete;

        static void initialize();

        static void registerWidget(const std::string& key, WidgetFactorySignature factory)
        {
            mFactories.insert({ key, factory });

            return;
        }

        static void createWidget(const std::string& key, WidgetManager& wm, const WidgetParameterMap& params)
        {
            mFactories.at(key)(wm, params);

            return;
        }

    private:
        static std::unordered_map<std::string, WidgetFactorySignature> mFactories;
        static bool mInitialized;
    };
}
