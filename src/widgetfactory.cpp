#include "conslr/widgetfactory.hpp"

#include "conslr/widgets/floatingtext.hpp"
#include "conslr/widgets/textbox.hpp"
#include "conslr/widgets/taggedfloatingtext.hpp"
#include "conslr/widgets/taggedtextbox.hpp"
#include "conslr/widgets/textinput.hpp"

std::unordered_map<std::string, conslr::WidgetFactorySignature> conslr::WidgetFactory::mFactories;
bool conslr::WidgetFactory::mInitialized = false;

void conslr::WidgetFactory::initialize()
{
    if (mInitialized) { return; }

    //Initialize packaged widgets
    registerWidget("FloatingText", widgets::constructFloatingText);
    registerWidget("TextBox", widgets::constructTextBox);
    registerWidget("TaggedFloatingText", widgets::constructTaggedFloatingText);
    registerWidget("TaggedTextBox", widgets::constructTaggedTextBox);
    registerWidget("TextInput", widgets::constructTextInput);

    mInitialized = true;
    return;
}
