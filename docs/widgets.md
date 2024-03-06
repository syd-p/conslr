# Using Widgets

1. Include the approriate header, i.e. TextBox is from `include/conslr/widgets/TextBox.hpp`

2. Create a screen to add widgets to and get the widget manager
```c++
int32_t screenIndex = console.createScreen();
console.setCurrentScreenIndex(screenIndex);

auto& widgetManager = console.getWidgetManager(screenIndex);
```
3. Create the widget and modify its data
```c++
auto widgetWeakPtr = widgetManager.createWidget<conslr::widgets::TextBox>(10);
{
    auto ptr = widgetWeakPtr.lock();
    ptr->setRegion({ 0, 0, 30, 10 });
    ptr->setString("Hello World!");
}
```
This pattern of getting the weak\_ptr from `WidgetManager::createWidget<T>` and then using a nameless scope to lock the pointer is what I personally prefer and recommend

# Creating Widgets From Json

1. Include the `include/conslr/widgetfactory.hpp` header to access the WidgetFactory

2. Create a json file to load
```json
{
    "Widgets":
    [
        {
            "type": "TextBox",
            "name": "myTextBox",
            "region": "0 0 30 10",
            "string": "Hello From Json!"
        }
    ]
}
```

3. Load the widgets through `WidgetManager::loadFromFile`
```c++
auto& widgetManager = console.getWidgetManager(screenIndex);
auto widgets = widgetManager.loadFromFile("my/path/file.json");
```

4. Get your specific widgets through the returned map
```c++
std::weak_ptr<conslr::Widget::TextBox> myTextBox = widgetManager.getWidget<conslr::Widget::TextBox>(widgets.at("myTextBox"));
```

# Extending WidgetFactory

1. Include the needed headers
```c++
#include <conslr/widgetmanager.hpp>
#include <conslr/widgetfactory.hpp>
#include <conslr/widget.hpp>
```

2. Create a widget
```c++
class MyWidget : public IWidget, public IRenderable
{
    friend class conslr::WidgetManager;
    friend class conslr::Screen;

    int32_t getMyMember() { return myMember; }
    void setMyMember(int32_t x) { myMember = x; }

protected:
    constexpr MyWidget(int32_t id, int32_t priority = 0) noexcept :
        IWidget{ id, priority }, IRenderable{}, 
        myMember{ 5 }
    {}

    int32_t myMember;
}
```

3. Create a constructor function

```c++
inline std::pair<std::string, int32_t>
    constructMyWidget(conslr::WidgetManager& wm, const conslr::WidgetParameterMap& params)
{
    auto weakPtr = wm.createWidget<MyWidget>();
    auto ptr = weakPtr.lock();

    if (params.contains("mymember"))
    {
        ptr->setMyMember(std::stoi(params.at("mymember")));
    }
    ...
}
```
4. Register the factory with WidgetFactory
```c++
WidgetFactory::registerWidget("mywidget", constructMyWidget);
```

5. Use your widget in a json file
```json
{
    "Widgets":
    [
        {
            "type": "mywidget",
            "name": "MyWidget1",
            "mymember": "10"
        }
    ]
}
```

See `include/conslr/widgets/...` for examples of this pattern
