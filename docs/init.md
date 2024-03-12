# Initializing the console

Some basic steps for creating a console in your app

1. Create the console (of course)
```c++
conslr::Console console{ cellWidth, cellHeight, windowCellWidth, windowCellHeight };
//Some properties of the console itself
console.setTitle("My Console!");
console.setIcon(...);
```

2. Load a font
```c++
int32_t font = console.createFont("myfont.bmp", fontCellWidth, fontCellHeight);
console.setCurrentFontIndex(font);
```

3. Create a screen
```c++
int32_t screen = console.createScreen();
console.setCurrentScreenIndex(screen);
```

Congrats, now you have a console that is set up and ready to be hold some widgets!

4. (Optional) Create or load a theme
```c++
//Loading a theme from file
conslr::Theme theme = console.createTheme("mytheme.txt");
console.setTheme(theme);

//Or create a theme programatically
conslr::Theme theme;
theme.background = { 255, 0, 0, 255 }; //Make the background red
console.setTheme(theme);
```

5. Loading some widgets

conslr is not very useful without widgets of course

My general philosophy around conslr is that the end user has the ability to choose either compile time or run time customization of themes and widgets.

Both methods of creating widgets are valid and remain up to the end user to decide the appropriate pattern to use

```c++
//Loading a screen from a file
auto& widgetManager = console.getWidgetManager(myScreenId);
auto myWidgets = widgetManager.loadFromFile("myfile.json"); //Loads from file and returns a map of the widgets
auto textbox = widgetManager.getWidget<conslr::widgets::TextBox>(myWidgets.at("MyTextBox1"));

//Creating them yourself
auto& widgetManager = console.getWidgetManager(myScreenId);
auto textbox = widgetManager.createWidget<conslr::widgets::TextBox>();
{
    //Setting up properties of the text box
    auto ptr = textbox.lock(); //Lock the weak ptr
    ptr->setString("Hello world!"); //Set the text
    ptr->setRegion({ 0, 0, 10, 10 }); //Set the region on screen of the textbox
}
```

# Compile-time vs Run-time?

Initializing things programatically vs at runtime has some positives and negatives, and I think its worth going over

Programatically

\+ Can be used to ensure a program will initialize correctly

\+ Faster initialization

\+ More portable, requires less external data to bootstrap it

\- Must be recompiled to test (although with caching it should not be too big of a time loss)

Run-time

\+ Can be created and tested quickly

\+ Added room for user customization that can still comply with the programs needs

\- Potential for breaking changes

\- Longer initialization times due to file i/o

I like both generally, although my preference thus far seems to lie as the following

Themes : Run-time

Widgets : Compile-time

# Your tools

conslr relies on 3 important components used to make functional programs

## conslr::Console

The console is effectively a window 

Its responsible only for things that may appear within that window

Such as the title of the window, the screens displayed on the window, etc

It does not by default have any information about what to render and how, and so this must be initialized

Most importantly this means that a console must be initialized with a font, and because its SDL this font is exclusive to that console only

Optionally it can also have a theme, although one is created by default

From here the console has screens which it may swap between to render, although all screens are effectively active at the same time

As such doing something on one screen can also have some effect on another screen, this would be accomplished though the next imporant component

## conslr::WidgetManager

Every screen that is created with `Console::createScreen` possess its own WidgetManager, which is used to actually render data to the screen

When a screen is rerendered, it goes to the WidgetManager and gets what rendering functions must be called and with what parameters and in what order (more on this later)

The screen itself lacks any functionality on its own, it provides render functions but its' WidgetManager is the thing that actually calls these functions

But WidgetManager on its own is also useless, it doesnt actually do anything because it has nothing to draw, for this there is the 3rd component

## Widgets

Widgets are a collection of classes with some shared features defined by conslr::IWidget

The 2 most important interfaces that define functionality of widgets are conslr::IWidget and conslr::IRenderable

As the name suggests a widget inheriting from IRenderable possesses a function render(conslr::Screen&)

This function contains the needed calls to a Screen to render the widget

The way rendering is determined is as such:

1. Console::render is called by the user

2. Console::render determines if the currently selected screen needs to be rerendered, if not it renders the screen as is, otherwise Screen::render is called

3. Screen::render clears the screen and then goes to its' WidgetManager to get the widgets that inherit from IRenderable

4. In order of lowest to highest priority, IRenderable::render is called on the widgets which then render themselves to the screen

## Using these tools

Now that I've went over the 3 core parts of conslr, I want to talk about how to use them

1. Store only the console and the needed widgets
  - There are exceptions to this, i.e. your main loop used WidgetManager functionality like activateWidget, but for the most part only needed widgets should be stored by your end of the program
  - Static widgets like textboxes that are never changed do not need to be stored, as it takes up space unnecessarily

2. Use unnamed or function scopes for initializing data
  - Suppose you want to create a textbox, first you recieve a weak\_ptr to the widget and then to modify it it must be locked with .lock(). Calling lock() for everything you want to said is tedious, so you could store it in a shared\_ptr instead and just lock it once, but if this is being done in say, main(), it defeats the purpose of the widget being a weak\_ptr in the first place, as such these should only be locked in a temporary scope, i.e. an unnamed scope in the initialization part of your program 
