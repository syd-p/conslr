#include <string>

#include <SDL.h>

#include <conslr/console.hpp>
#include <conslr/widgetfactory.hpp>
#include <conslr/widgetmanager.hpp>
#include <conslr/widgets/textbox.hpp>
#include <conslr/widgets/scrolllist.hpp>

int main()
{
    conslr::Console console{ 16, 32, 80, 20 };

    int32_t scr = console.createScreen();
    console.setCurrentScreenIndex(scr);

    int32_t font = console.createFont("res/ibm_vga_fontsheet.bmp", 8, 16);
    console.setCurrentFontIndex(font);

    auto widgets = console.getWidgetManager(scr).loadFromFile("res/audioplayer.json");
    auto playingText = console.getWidgetManager(scr).getWidget<conslr::widgets::TextBox>(widgets.at("SongInfo"));
    auto songList = console.getWidgetManager(scr).getWidget<conslr::widgets::ScrollList<std::string>>(widgets.at("SongList"));
    
    bool running = true;
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        console.render();
    }

    console.destroy();

    return 0;
}
