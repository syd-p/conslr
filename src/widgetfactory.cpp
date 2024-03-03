#include "conslr/widgetfactory.hpp"

#include "conslr/widgets/floatingtext.hpp"

std::unordered_map<std::string, conslr::WidgetFactorySignature> conslr::WidgetFactory::mFactories;
bool conslr::WidgetFactory::mInitialized = false;

void conslr::WidgetFactory::initialize()
{
    if (mInitialized) { return; }

    //Initialize packaged widgets
    registerWidget("FloatingText", widgets::constructFloatingText);

    mInitialized = true;
    return;
}
