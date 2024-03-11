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
conslr::Theme theme{ conslr::loadThemeFromFile("res/themes/sometheme.txt") };
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
