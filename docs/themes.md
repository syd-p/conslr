Themes in conslr are represented and passed around as a shared\_ptr to a theme

Some info to keep track of:

- Setting the Console theme recursively sets the screen and widgets themes

- Setting the WidgetManager theme recursively sets the widgets themes

- Setting themes of the WidgetManager or Widget require a shared\_ptr to be passed to it

- Setting the theme of Console takes a const Theme& and instead creates a new shared\_ptr

- Theme::extended is user implemented

    - Theme.extended is a raw pointer !!

- Themes are loaded from files, see the loadfromfile example and res/themes/
