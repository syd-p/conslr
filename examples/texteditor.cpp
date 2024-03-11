///A text editor program
///
///Controls
///I to edit text
///Escape to return to menu
///Enter to select menu options
///Enter to confirm file name
#include <ios>
#include <iterator>
#include <memory>
#include <string>
#include <fstream>

#include <SDL.h>

#include <conslr/console.hpp>
#include <conslr/theme.hpp>
#include <conslr/widgets/textbox.hpp>
#include <conslr/widgets/textinput.hpp>
#include <conslr/widgets/scrolllist.hpp>

using namespace conslr;

enum class MenuOptions
{
    New,
    Save,
    SaveAs,
    Open,
    Exit
};

enum class AppState
{
    Editing,
    Options,
    EnterFile
};

int main()
{
    Console console{ 16, 32, 80, 24 };
    console.setTitle("Text Editor");

    int32_t font = console.createFont("res/ibm_vga_fontsheet.bmp", 8, 16);
    console.setCurrentFontIndex(font);

    int32_t mainScr = console.createScreen();
    console.setCurrentScreenIndex(mainScr);

    console.setTheme(conslr::Theme{ "res/themes/turboblue.txt" });

    //Create main screen
    auto& mainWm = console.getWidgetManager(mainScr);
    auto text = mainWm.createWidget<widgets::TextInput>(10);
    {
        auto ptr = text.lock();
        ptr->setTitle("Untitled.txt");
        ptr->showTitle();
        ptr->setRegion({ 0, 0, 60, 24 });
    }

    auto menu = mainWm.createWidget<widgets::ScrollList<MenuOptions>>(10);
    {
        auto ptr = menu.lock();
        ptr->setTitle("Options");
        ptr->showTitle();
        ptr->setRegion({ 60, 0, 20, 24 });
        ptr->setActive(true);
        
        ptr->addElement(MenuOptions::New, "New");
        ptr->addElement(MenuOptions::Open, "Open");
        ptr->addElement(MenuOptions::Save, "Save");
        ptr->addElement(MenuOptions::SaveAs, "Save As");
        ptr->addElement(MenuOptions::Exit, "Exit");
    }

    auto optionsInput = mainWm.createWidget<widgets::TextInput>(12);
    {
        auto ptr = optionsInput.lock();
        ptr->setTitle("Enter File Name");
        ptr->showTitle();
        ptr->hide();
        ptr->setMaxRows(1);
        ptr->setRegion({ 10, 8, 40, 3 });
    }

    std::string fileName = "Untitled.txt";

    SDL_StopTextInput();
    AppState state = AppState::Options;
    MenuOptions chosenOption = MenuOptions::New;
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

            if (event.type == SDL_TEXTINPUT)
            {                
                assert(state == AppState::Editing || state == AppState::EnterFile);

                if (state == AppState::Editing)
                {
                    text.lock()->doTextInput(event.text);
                }
                else if (state == AppState::EnterFile)
                {
                    optionsInput.lock()->doTextInput(event.text);
                }
            }

            if (event.type == SDL_KEYDOWN)
            {
                SDL_Scancode sc = event.key.keysym.scancode;

                if (state == AppState::Options)
                {
                    if (sc == SDL_SCANCODE_I)
                    {
                        //Begin editing
                        state = AppState::Editing;
                        mainWm.deactivateWidget(menu.lock()->getId());
                        mainWm.activateWidget(text.lock()->getId());
                        SDL_StartTextInput();
                    }
                    else if (sc == SDL_SCANCODE_UP) { menu.lock()->scrollUp(); }
                    else if (sc == SDL_SCANCODE_DOWN) { menu.lock()->scrollDown(); }
                    else if (sc == SDL_SCANCODE_RETURN)
                    {
                        //Choose option
                        auto option = menu.lock()->getCurrentElement().mElement;

                        if (option == MenuOptions::New)
                        {
                            state = AppState::EnterFile;
                            chosenOption = option;

                            mainWm.deactivateWidget(menu.lock()->getId());
                            mainWm.activateWidget(optionsInput.lock()->getId());
                            optionsInput.lock()->show();
                            optionsInput.lock()->setString("");

                            SDL_StartTextInput();
                        }
                        else if (option == MenuOptions::Open)
                        {
                            state = AppState::EnterFile;
                            chosenOption = option;

                            mainWm.deactivateWidget(menu.lock()->getId());
                            mainWm.activateWidget(optionsInput.lock()->getId());
                            optionsInput.lock()->show();
                            optionsInput.lock()->setString("");

                            SDL_StartTextInput();

                        }
                        else if (option == MenuOptions::Save)
                        {
                            std::ofstream ofs(fileName, std::ios_base::trunc);
                            if (ofs.good())
                            {
                                ofs << text.lock()->getString();
                                ofs.close();
                            }
                       }
                        else if (option == MenuOptions::SaveAs)
                        {
                            state = AppState::EnterFile;
                            chosenOption = option;

                            mainWm.deactivateWidget(menu.lock()->getId());
                            mainWm.activateWidget(optionsInput.lock()->getId());
                            optionsInput.lock()->show();
                            optionsInput.lock()->setString("");

                            SDL_StartTextInput();
                        }
                        else if (option == MenuOptions::Exit)
                        {
                            running = false;
                        }
                    }
                }
                else if (state == AppState::Editing)
                {
                    if (sc == SDL_SCANCODE_ESCAPE)
                    {
                        //End editing
                        state = AppState::Options;
                        mainWm.activateWidget(menu.lock()->getId());
                        mainWm.deactivateWidget(text.lock()->getId());
                        SDL_StopTextInput();
                    }
                    else if (sc == SDL_SCANCODE_UP) { text.lock()->doKeyUp(); }
                    else if (sc == SDL_SCANCODE_DOWN) { text.lock()->doKeyDown(); }
                    else if (sc == SDL_SCANCODE_LEFT) { text.lock()->doKeyLeft(); }
                    else if (sc == SDL_SCANCODE_RIGHT) { text.lock()->doKeyRight(); }
                    else if (sc == SDL_SCANCODE_TAB) { text.lock()->doTab(); }
                    else if (sc == SDL_SCANCODE_RETURN) { text.lock()->doReturn(); }
                    else if (sc == SDL_SCANCODE_BACKSPACE) { text.lock()->doBackspace(); } 
                }
                else if (state == AppState::EnterFile)
                {
                    if (sc == SDL_SCANCODE_ESCAPE)
                    {
                        //Exit without doing option
                        state = AppState::Options;

                        mainWm.activateWidget(menu.lock()->getId());
                        mainWm.deactivateWidget(optionsInput.lock()->getId());
                        optionsInput.lock()->hide();

                        SDL_StopTextInput();
                    }
                    else if (sc == SDL_SCANCODE_RETURN)
                    {
                        //Exit and do option
                        if (chosenOption == MenuOptions::New)
                        {
                            fileName = optionsInput.lock()->getString();
                            state = AppState::Options;

                            mainWm.activateWidget(menu.lock()->getId());
                            mainWm.deactivateWidget(optionsInput.lock()->getId());
                            optionsInput.lock()->hide();
                            text.lock()->setTitle(fileName);
                            text.lock()->setString("");

                            SDL_StopTextInput();
                        }
                        else if (chosenOption == MenuOptions::Open)
                        {
                            fileName = optionsInput.lock()->getString();
                            state = AppState::Options;

                            mainWm.activateWidget(menu.lock()->getId());
                            mainWm.deactivateWidget(optionsInput.lock()->getId());
                            optionsInput.lock()->hide();
                            text.lock()->setTitle(fileName);
                            
                            SDL_StopTextInput();

                            std::ifstream ifs(fileName);
                            if (ifs.good())
                            {
                                std::string str
                                {
                                    std::istreambuf_iterator<char>(ifs),
                                    std::istreambuf_iterator<char>()
                                };

                                text.lock()->setString(str);
                                ifs.close();
                            }
                            else
                            {
                                text.lock()->setString("Failed to open file: " + fileName);
                                ifs.close();
                            }
                           
                        }
                        else if (chosenOption == MenuOptions::SaveAs)
                        {
                            fileName = optionsInput.lock()->getString();
                            state = AppState::Options;

                            mainWm.activateWidget(menu.lock()->getId());
                            mainWm.deactivateWidget(optionsInput.lock()->getId());
                            optionsInput.lock()->hide();
                            text.lock()->setTitle(fileName);
                            
                            SDL_StopTextInput();

                            std::ofstream ofs(fileName, std::ios_base::trunc);
                            if (ofs.good())
                            {
                                ofs << text.lock()->getString();
                                ofs.close();
                            }
                        }
                    }
                    else if (sc == SDL_SCANCODE_LEFT) { optionsInput.lock()->doKeyLeft(); }
                    else if (sc == SDL_SCANCODE_RIGHT) { optionsInput.lock()->doKeyRight(); }
                    else if (sc == SDL_SCANCODE_BACKSPACE) { optionsInput.lock()->doBackspace(); }
                }
            }

            console.doEvent(event);
        }

        console.render();
    }

    SDL_StopTextInput();

    return 0;
}
