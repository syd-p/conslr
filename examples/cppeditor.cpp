#include <fstream>
#include <string>
#include <sstream>

#include <SDL.h>
#include <conslr/console.hpp>

#include <conslr/themes/turboblue.hpp>

#include <conslr/widgets/textinput.hpp>
#include <conslr/widgets/taggedfloatingtext.hpp>

void save(const std::string& str)
{
    std::ofstream ofs("editor/file.cpp", std::ios::trunc);

    ofs << str;

    ofs.close();

    return;
}

std::string load()
{
    std::ifstream ifs("editor/file.cpp");
    if (!ifs.good()) { return ""; }

    std::stringstream ss;
    ss << ifs.rdbuf();

    ifs.close();

    return ss.str();
}

void build()
{
    system("g++ editor/file.cpp -o editor/output");

    return;
}

void run()
{
    system("./editor/output");

    return;
}

int main()
{
    //Init console
    conslr::Console console{ 16, 32, 80, 24 };
    console.setTitle("C++ Editor");

    int32_t scr = console.createScreen();
    console.setCurrentScreenIndex(scr);

    int32_t font = console.createFont("ibm_vga_fontsheet.bmp", 8, 16);
    console.setCurrentFontIndex(font);

    console.setTheme(conslr::themes::TurboBlue);

    auto& wm = console.getWidgetManager(scr);

    auto textInput = wm.createWidget<conslr::widgets::TextInput>(10);
    {
        auto ptr = textInput.lock();
        ptr->setRegion({ 0, 0, 79, 23 });
    }

    SDL_Event event;
    bool editing = false;
    bool running = true;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }

            if (editing)
            {
                //Editing
                if (event.type == SDL_KEYDOWN)
                {
                    const auto& keys = console.getKeyMap();

                    if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    {
                        editing = false;
                        SDL_StopTextInput();
                        console.getWidgetManager(scr).deactivateWidget(textInput.lock()->getId());
                    }
                    else if (event.key.keysym == keys.scrollUp) { textInput.lock()->doKeyUp(); }
                    else if (event.key.keysym == keys.scrollDown) { textInput.lock()->doKeyDown(); }
                    else if (event.key.keysym == keys.scrollLeft) { textInput.lock()->doKeyLeft(); }
                    else if (event.key.keysym == keys.scrollRight) { textInput.lock()->doKeyRight(); }
                    else if (event.key.keysym == keys.tab) { textInput.lock()->doTab(); }
                    else if (event.key.keysym == keys.backspace) { textInput.lock()->doBackspace(); }
                    else if (event.key.keysym == keys.enter) { textInput.lock()->doReturn(); }
                }
                else if (event.type == SDL_TEXTINPUT)
                {
                    textInput.lock()->doTextInput(event.text);
                }
            }
            else
            {
                //Not editing
                if (event.type == SDL_KEYDOWN)
                {
                    if (event.key.keysym.scancode == SDL_SCANCODE_I)
                    {
                        editing = true;
                        SDL_StartTextInput();
                        console.getWidgetManager(scr).activateWidget(textInput.lock()->getId());
                    }
                    else if (event.key.keysym.scancode == SDL_SCANCODE_B)
                    {
                        //Build
                        save(textInput.lock()->getString());
                        build();
                    }
                    else if (event.key.keysym.scancode == SDL_SCANCODE_R)
                    {
                        //Build and run
                        save(textInput.lock()->getString());
                        build();
                        run();
                    }
                    else if (event.key.keysym.scancode == SDL_SCANCODE_S)
                    {
                        //Save
                        save(textInput.lock()->getString());
                    }
                    else if (event.key.keysym.scancode == SDL_SCANCODE_L)
                    {
                        //Load
                        textInput.lock()->setString(load());
                    }
                }
            }

            console.doEvent(event);
        }

        console.render();
    }

    return 0;
}
