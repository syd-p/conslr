#include <filesystem>

#include <SDL.h>

#include <conslr/console.hpp>
#include <conslr/widgetmanager.hpp>
#include <conslr/widgetfactory.hpp>
#include <conslr/widgets/textbox.hpp>
#include <conslr/widgets/scrolllist.hpp>

int main()
{
    conslr::Console console{ 16, 32, 80, 20 };

    int32_t scr = console.createScreen();
    console.setCurrentScreenIndex(scr);

    int32_t font = console.createFont("res/ibm_vga_fontsheet.bmp", 8, 16);
    console.setCurrentFontIndex(font);

    auto widgets = console.getWidgetManager(scr).loadFromFile("./res/audioplayer.json");
    auto infoBox = console.getWidgetManager(scr).getWidget<conslr::widgets::TextBox>(widgets.at("SongInfo"));
    auto audioList = console.getWidgetManager(scr).getWidget<conslr::widgets::ScrollList<std::string>>(widgets.at("SongList"));

    for (const auto& entry : std::filesystem::directory_iterator("songs/"))
    {
        if (entry.is_regular_file())
        {
            audioList.lock()->addElement(entry.path().string(), entry.path().filename().string());
        }
    }

    SDL_Event event;
    bool running = true;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }

            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.scancode == SDL_SCANCODE_UP)
                {
                    audioList.lock()->scrollUp();
                }

                if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
                {
                    audioList.lock()->scrollDown();
                }

                if (event.key.keysym.scancode == SDL_SCANCODE_RETURN)
                {
                    //Play file
                }
            }

            console.doEvent(event);
        }

        console.render();
    }

    console.destroy();
    SDL_Quit();

    return 0;
}
