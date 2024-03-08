#include "conslr/widgetfactory.hpp"

#include "conslr/widgets/floatingtext.hpp"
#include "conslr/widgets/textbox.hpp"
#include "conslr/widgets/taggedfloatingtext.hpp"
#include "conslr/widgets/taggedtextbox.hpp"
#include "conslr/widgets/textinput.hpp"
#include "conslr/widgets/scrolllist.hpp"
#include "conslr/widgets/radiolist.hpp"
#include "conslr/widgets/checklist.hpp"
#include "conslr/widgets/progressbar.hpp"

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
    registerWidget("ProgressBar", widgets::constructProgressBar);

    //Some template ones
    registerWidget("ScrollListInt", widgets::constructScrollList<int32_t>);
    registerWidget("ScrollListUInt", widgets::constructScrollList<uint32_t>);
    registerWidget("ScrollListDouble", widgets::constructScrollList<double>);
    registerWidget("ScrollListStr", widgets::constructScrollList<std::string>);
    registerWidget("ScrollListChar", widgets::constructScrollList<char>);

    registerWidget("RadioListInt", widgets::constructRadioList<int32_t>);
    registerWidget("RadioListUInt", widgets::constructRadioList<uint32_t>);
    registerWidget("RadioListDouble", widgets::constructRadioList<double>);
    registerWidget("RadioListStr", widgets::constructRadioList<std::string>);
    registerWidget("RadioListChar", widgets::constructRadioList<char>);

    registerWidget("CheckListInt", widgets::constructCheckList<int32_t>);
    registerWidget("CheckListUInt", widgets::constructCheckList<uint32_t>);
    registerWidget("CheckListDouble", widgets::constructCheckList<double>);
    registerWidget("CheckListStr", widgets::constructCheckList<std::string>);
    registerWidget("CheckListChar", widgets::constructCheckList<char>);

    mInitialized = true;
    return;
}
