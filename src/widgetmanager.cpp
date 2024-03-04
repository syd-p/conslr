#include "conslr/widgetmanager.hpp"

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <fstream>

#include <nlohmann/json.hpp>

#include "conslr/widgetfactory.hpp"

conslr::WidgetManager::WidgetManager()
{
    for (int i = 0; i < MAX_WIDGETS; i++)
    {
        mFreeWidgets.push(i);
        mWidgets.at(i) = nullptr;
    }

    return;
}

void conslr::WidgetManager::destroyWidget(int32_t index)
{
    if (!(index >= 0 && index < MAX_WIDGETS))
    {
        throw std::invalid_argument("Widget index is out of bounds, index: " + std::to_string(index));
    }
    if (mWidgets.at(index) == nullptr)
    {
        throw std::runtime_error("Widget at index is already nullptr, index: " + std::to_string(index));
    }

    mWidgets.at(index) = nullptr;

    for (auto it = mRenderable.begin(); it != mRenderable.end(); it++)
    {
        if (std::dynamic_pointer_cast<IWidget>(*it)->getId() == index)
        {
            mRenderable.erase(it);
            break;
        }
    }

    mFreeWidgets.push(index);

    return;
}

void conslr::WidgetManager::activateWidget(int32_t index)
{
    if (!(index >= 0 && index < MAX_WIDGETS))
    {
        throw std::invalid_argument("Widget index is out of bounds, index: " + std::to_string(index));
    }
    if (mWidgets.at(index) == nullptr)
    {
        throw std::invalid_argument("Widget at index is nullptr, index: " + std::to_string(index));
    }

    for (auto renderablePtr : mRenderable)
    {
        renderablePtr->mRerender = true;
    }

    mWidgets.at(index)->setActive(true);

    return;
}

void conslr::WidgetManager::deactivateWidget(int32_t index)
{
    if (!(index >= 0 && index < MAX_WIDGETS))
    {
        throw std::invalid_argument("Widget index is out of bounds, index: " + std::to_string(index));
    }
    if (mWidgets.at(index) == nullptr)
    {
        throw std::runtime_error("Widget at index is nullptr, index: " + std::to_string(index));
    }

    for (auto renderablePtr : mRenderable)
    {
        renderablePtr->mRerender = true;
    }

    mWidgets.at(index)->setActive(false);

    return;
}

//Loads the file into json, assuming its formed correctly
//Data from the json file gets passed in the form of [parameter, value], where parameter is used to modify the widget in some way
//The WidgetFactory takes the [param, value] pairs and creates the widget through WidgetManager
//It then modifies the widget according the pairs
//Widget types have to be given a function to construct them from the json file
//This is provided for the default widgets in the WidgetFactory::initialize method
std::unordered_map<std::string, int32_t> conslr::WidgetManager::loadFromFile(const std::string& file)
{
    std::ifstream ifs(file);
    if (!ifs.good())
    {
        throw std::runtime_error("Failed to open file, file: " + file);
    }

    nlohmann::json data = nlohmann::json::parse(ifs);
    ifs.close();

    clear();
    WidgetFactory::initialize();
    std::unordered_map<std::string, int32_t> widgetNames;

    for (auto& widget : data["Widgets"].items())
    {
        std::unordered_map<std::string, std::string> params;

        for (auto& param : widget.value().items())
        {
            params[param.key()] = param.value();
        }

        if (!params.contains("type"))
        {
            throw std::runtime_error("Widget does not have specified type, file: " + file);
        }

        auto name = WidgetFactory::createWidget(params.at("type"), *this, params);        

        //No copies found
        if (!widgetNames.contains(name.first))
        {
            widgetNames.insert(name);
            continue;
        }

        int32_t copyIndex = 1;
        while (widgetNames.contains(name.first + '(' + std::to_string(copyIndex) + ')'))
        {
            copyIndex++;
        }

        widgetNames.insert({ name.first + '(' + std::to_string(copyIndex) + ')', name.second });
    }

    return widgetNames;
}

void conslr::WidgetManager::clear()
{
    std::queue<int32_t> empty;
    std::swap(mFreeWidgets, empty);
    for (auto i = 0; i < MAX_WIDGETS; i++)
    {
        mFreeWidgets.push(i);
    }
    
    std::array<std::shared_ptr<IWidget>, MAX_WIDGETS> emptyArr{};
    std::swap(mWidgets, emptyArr);

    mRenderable.clear();

    return;
}
