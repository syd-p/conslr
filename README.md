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
## Loading Widgets From File
Widgets can optionally be loaded using the WidgetFactory::createWidget function where the key is the type of widget, wm is the WidgetManager to create it with, and params is a map of parameters to create the widget

Although it is probably simpler to do it manually, the options there

Mainly WidgetFactory exists to interface with WidgetManager in creating widgets from a file

To construct a Widget using the WidgetFactory (and by extenstion through json) you must register a constructor function with a key which is typically the type of widget

By default, the built-in widgets are all registered with WidgetFactory automatically

```c++
console.getWidgetManager(index).loadFromFile("somefile.json"); //Loading through WidgetManager
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
void constructMyWidget(WidgetManager& wm, const WidgetParameterMap& params)
{ ... }

int main()
{
  //Register your constructor
  //"MyWidget" can now be used a key to call WidgetFactory::createWidget with 
  WidgetFactory::registerWidget("MyWidget", constructMyWidget);
}
```
