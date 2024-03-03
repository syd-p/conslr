#include "conslr/widgetfactory.hpp"

#include "conslr/widgets/textbox.hpp"

std::unordered_map<std::string, conslr::WidgetFactorySignature> conslr::WidgetFactory::mFactories;
bool conslr::WidgetFactory::mInitialized = false;

void conslr::WidgetFactory::initialize()
{
    if (mInitialized) { return; }

    //Initialize packaged widgets
    registerWidget("TextBox", [](WidgetManager& wm, const WidgetParameterMap& params)
            {
                auto wptr = wm.createWidget<widgets::TextBox>();
                auto ptr = wptr.lock();
                ptr->setRegion({ 0, 0, 10, 10 });
                ptr->setString("Hello world");
            });

    mInitialized = true;
    return;
}
