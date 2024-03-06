# conslr - A Widget Based Terminal Emulator
This is a pet project of mine initially created as an abstraction layer to a game I am working on.

It is meant to be relatively extendable and easily modifiable.

![Example of conslr being used](https://github.com/syd-p/conslr/blob/master/res/widgetsexample.png)

## Installation
As there are no precompiled binaries (yet), you have to build it from source

To do so do the following
1. Clone the repo:
```
git clone https://github.com/syd-p/conslr.git
cd conslr
```
2. Change to the build directory and run build.sh 
```
cd build
./build.sh
```
3. Install:
```
cmake --install .
```
## Getting Started
First include the needed headers
```c++
#include <conslr/console.hpp> //For the console itself
#include <conslr/widgetmanager.hpp> //For the WidgetManager
#include <conslr/widgets/textbox.hpp> //A widget to add
```

Then create a console with a simple loop
```c++
conslr::Console console{ 16, 32, 80, 24 }; //Create a console where the cells are 16x32 and the console size is 80x24
console.setTitle("Hello world!"); //Set the title of the window

int32_t screen = console.createScreen();
console.setCurrentScreenIndex(screen);

int32_t font = console.createFont("Font/Path/font.bmp", 8, 16);
console.setCurrentFontIndex(font);

auto& widgetManager = console.getWidgetManager(screen); //Get the WidgetManager for the screen
auto textbox = widgetManager.createWidget<conslr::widgets::TextBox>(0); //Create the widget with priority 0
textbox.lock()->setRegion({ 0, 0, 40, 12 }); //Set the region of the widget
textbox.lock()->setString("This is a textbox!"); //Set the text of the widget

SDL_Event event; //This is build on SDL, so it uses a loop similar to other SDL programs
bool running = true;
while (running)
{
    while (SDL_PollEvent(&event)) //Loop through all events
    {
        if (event.type == SDL_QUIT)
        {
            running = false; //Exit the loop
        }

        console.doEvent(event);
    }

    console.render(); //Render the console
}
```

## Loading Widgets From File
Widgets can optionally be loaded using the WidgetFactory::createWidget function where the key is the type of widget, wm is the WidgetManager to create it with, and params is a map of parameters to create the widget

Although it is probably simpler to do it manually, the options there

Mainly WidgetFactory exists to interface with WidgetManager in creating widgets from a file

To construct a Widget using the WidgetFactory you must register a constructor function with a key which is typically the type of widget

By default, the built-in widgets are all registered with WidgetFactory automatically

```c++
auto widgetNames = console.getWidgetManager(index).loadFromFile("somefile.json"); //Loading through WidgetManager
```

```c++
//Creating using the factory directly
WidgetFactory::createWidget("TextBox", console.getWidgetManager(index),
  {
    { "priority", "10" },
    { "string", "Hello World!" }
  });
```
```c++
//Some class you made
class MyWidget : public IWidget, public IRenderable { ... }
//Function which constructs a widget using params
std::pair<std::string, int32_t> constructMyWidget(WidgetManager& wm, const WidgetParameterMap& params)
{ 
    ... 
    return { widgetName, widgetId };
}

int main()
{
  //Register your constructor
  //"MyWidget" can now be used a key to call WidgetFactory::createWidget with 
  WidgetFactory::registerWidget("MyWidget", constructMyWidget);
}
```
