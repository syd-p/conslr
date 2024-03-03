#include "conslr/widgetmanager.hpp"

#include <memory>
#include <utility>

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
    assert((index >= 0 && index < MAX_WIDGETS) && "Index out of bounds");
    assert((mWidgets.at(index) != nullptr) && "Widget does not exist");

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
    assert((index >= 0 && index < MAX_WIDGETS) && "Index out of bounds");
    assert((mWidgets.at(index) != nullptr) && "Widget does not exist");

    for (auto renderablePtr : mRenderable)
    {
        renderablePtr->mRerender = true;
    }

    mWidgets.at(index)->setActive(true);

    return;
}

void conslr::WidgetManager::deactivateWidget(int32_t index)
{
    assert((index >= 0 && index < MAX_WIDGETS) && "Index out of bounds");
    assert((mWidgets.at(index) != nullptr) && "Widget does not exist");

    for (auto renderablePtr : mRenderable)
    {
        renderablePtr->mRerender = true;
    }

    mWidgets.at(index)->setActive(false);

    return;
}

void conslr::WidgetManager::loadFromFile(const std::string& file)
{
    clear();
    WidgetFactory::initialize();

    //Todo Add File Loading

    WidgetFactory::createWidget("FloatingText", *this, 
            {
            { "region", "0 0 20 4" },
            { "string", "Hello world!" }
            });

    return;
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
