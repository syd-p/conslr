#include "conslr/widgets/checklist.hpp"
#include "conslr/widgets/progressbar.hpp"
#include "conslr/widgets/textinput.hpp"
#include <algorithm>

#include <SDL.h>

#include <conslr/console.hpp>
#include <conslr/theme.hpp>
#include <conslr/widgetmanager.hpp>
#include <conslr/widgets/floatingtext.hpp>
#include <conslr/widgets/scrolllist.hpp>
#include <conslr/widgets/textbox.hpp>

enum class ScreenState
{
    colors,
    characters,
    widgets
};

enum class ThemeColors 
{
    background,
    text,
    border,
    selection
};

enum class ThemeCharacters
{
    borderHorizontal,
    borderVertical,
    borderCornerTl,
    borderCornerTr,
    borderCornerBl,
    borderCornerBr,
    scrollbarCharacter,
    scrollbarCharacterHorizontal,
    selectionTick,
    progress
};

SDL_Color& getColor(conslr::Theme& theme, ThemeColors color)
{
    switch (color)
    {
    case ThemeColors::background:
        return theme.background;
    case ThemeColors::text:
        return theme.text;
    case ThemeColors::border:
        return theme.border;
    case ThemeColors::selection:
        return theme.selection;
    default:
        return theme.background;
    }
}

int32_t& getCharacter(conslr::Theme& theme, ThemeCharacters character)
{
    switch (character)
    {
    case ThemeCharacters::borderHorizontal:
        return theme.borderHorizontal;
    case ThemeCharacters::borderVertical:
        return theme.borderVertical;
    case ThemeCharacters::borderCornerTl:
        return theme.borderCornerTl;
    case ThemeCharacters::borderCornerTr:
        return theme.borderCornerTr;
    case ThemeCharacters::borderCornerBl:
        return theme.borderCornerBl;
    case ThemeCharacters::borderCornerBr:
        return theme.borderCornerBr;
    case ThemeCharacters::scrollbarCharacter:
        return theme.scrollbarCharacter;
    case ThemeCharacters::scrollbarCharacterHorizontal:
        return theme.scrollbarCharacterHorizontal;
    case ThemeCharacters::selectionTick:
        return theme.selectionTick;
    case ThemeCharacters::progress:
        return theme.progress;
    default:
        return theme.borderHorizontal;
    }
}

int main()
{
    conslr::Console console{ 16, 32, 80, 24 };
    console.setTitle("Theme editor");

    int32_t font = console.createFont("res/ibm_vga_fontsheet.bmp", 8, 16);
    console.setCurrentFontIndex(font);

    int32_t colorScr = console.createScreen();
    int32_t charScr = console.createScreen();
    int32_t widgetScr = console.createScreen();
    console.setCurrentScreenIndex(colorScr);

    conslr::Theme theme;
    console.setTheme(theme);

    auto& colorWm = console.getWidgetManager(colorScr);
    auto colorHeader = colorWm.createWidget<conslr::widgets::FloatingText>();
    {
        auto ptr = colorHeader.lock();
        ptr->setRegion({ 0, 0, 80, 1 });
        ptr->setString("[1 - Colors] 2 - Characters  3 - Widgets ");
    }

    auto colorList = colorWm.createWidget<conslr::widgets::ScrollList<ThemeColors>>();
    {
        auto ptr = colorList.lock();
        ptr->setRegion({ 0, 1, 60, 23 });
        ptr->setTitle("Colors");
        ptr->showTitle();
        ptr->setActive(true);

        ptr->addElement(ThemeColors::background, "Background");
        ptr->addElement(ThemeColors::text, "Text");
        ptr->addElement(ThemeColors::border, "Border");
        ptr->addElement(ThemeColors::selection, "Selection");
    }

    auto currentColor = colorWm.createWidget<conslr::widgets::TextBox>();
    {
        auto ptr = currentColor.lock();
        ptr->setRegion({ 60, 1, 20, 4 });
        ptr->setString("Background");
    }

    auto colorR = colorWm.createWidget<conslr::widgets::FloatingText>();
    {
        auto ptr = colorR.lock();
        ptr->setRegion({ 60, 6, 5, 1 });
        ptr->setString(" " + std::to_string(getColor(theme, ThemeColors::background).r) + " ");
    }

    auto colorG = colorWm.createWidget<conslr::widgets::FloatingText>();
    {
        auto ptr = colorG.lock();
        ptr->setRegion({ 60, 8, 5, 1 });
        ptr->setString(" " + std::to_string(getColor(theme, ThemeColors::background).g) + " ");
    }

    auto colorB = colorWm.createWidget<conslr::widgets::FloatingText>();
    {
        auto ptr = colorB.lock();
        ptr->setRegion({ 60, 10, 5, 1 });
        ptr->setString(" " + std::to_string(getColor(theme, ThemeColors::background).b) + " ");
    }

    auto colorA = colorWm.createWidget<conslr::widgets::FloatingText>();
    {
        auto ptr = colorA.lock();
        ptr->setRegion({ 60, 12, 5, 1 });
        ptr->setString(" " + std::to_string(getColor(theme, ThemeColors::background).a) + " ");
    }

    auto& charWm  = console.getWidgetManager(charScr);
    auto charHeader = charWm.createWidget<conslr::widgets::FloatingText>();
    {
        auto ptr = charHeader.lock();
        ptr->setRegion({ 0, 0, 80, 1 });
        ptr->setString(" 1 - Colors [2 - Characters] 3 - Widgets ");
    }

    auto charList = charWm.createWidget<conslr::widgets::ScrollList<ThemeCharacters>>();
    {
        auto ptr = charList.lock();
        ptr->setRegion({ 0, 1, 60, 23 });
        ptr->setTitle("Characters");
        ptr->showTitle();
        ptr->setActive(true);

        ptr->addElement(ThemeCharacters::borderHorizontal, "Border Horizontal");
        ptr->addElement(ThemeCharacters::borderVertical, "Border Vertical");
        ptr->addElement(ThemeCharacters::borderCornerTl, "Border Top Left");
        ptr->addElement(ThemeCharacters::borderCornerTr, "Border Top Right");
        ptr->addElement(ThemeCharacters::borderCornerBl, "Border Bottom Left");
        ptr->addElement(ThemeCharacters::borderCornerBr, "Border Bottom Right");
        ptr->addElement(ThemeCharacters::scrollbarCharacter, "Scroll Bar");
        ptr->addElement(ThemeCharacters::scrollbarCharacterHorizontal, "Scroll Bar Horizontal");
        ptr->addElement(ThemeCharacters::selectionTick, "Selection Tick");
        ptr->addElement(ThemeCharacters::progress, "Progress Bar");
    }

    auto currentChar = charWm.createWidget<conslr::widgets::TextBox>();
    {
        auto ptr = currentChar.lock();
        ptr->setRegion({ 60, 1, 20, 4 });
        ptr->setString("Border Horizontal");
    }

    auto charSlider = charWm.createWidget<conslr::widgets::FloatingText>();
    {
        auto ptr = charSlider.lock();
        ptr->setRegion({ 60, 6, 5, 1 });
        ptr->setString(" " + std::to_string(getCharacter(theme, ThemeCharacters::borderHorizontal)) + " ");
    }

    auto& widgetsWm = console.getWidgetManager(widgetScr);
    auto widgetHeader = widgetsWm.createWidget<conslr::widgets::FloatingText>();
    {
        auto ptr = widgetHeader.lock();
        ptr->setRegion({ 0, 0, 80, 1 });
        ptr->setString(" 1 - Colors  2 - Characters [3 - Widgets]");
    }

    auto wList = widgetsWm.createWidget<conslr::widgets::CheckList<int32_t>>();
    {
        auto ptr = wList.lock();
        ptr->setRegion({ 0, 1, 30, 23 });
        ptr->setActive(true);
        
        for (auto i = 0; i < 40; i++)
        {
            ptr->addElement({ i, false }, std::to_string(i));
        }
    }

    auto wInput = widgetsWm.createWidget<conslr::widgets::TextInput>();
    {
        auto ptr = wInput.lock();
        ptr->setRegion({ 30, 1, 8, 3 });
        ptr->setString("ABCDEFGHIJKLMNOP Q R S T U V");
    }

    auto wProgress = widgetsWm.createWidget<conslr::widgets::ProgressBar>();
    {
        auto ptr = wProgress.lock();
        ptr->setRegion({ 30, 9, 20, 5 });
        ptr->setCurrentValue(50);
    }

    ScreenState state = ScreenState::colors;

    ThemeColors colorOption = ThemeColors::background; //Currently editing
    int32_t rgbaSelection = 0; //0-3, rgba
    bool editingColor = false; //Selecting color first

    ThemeCharacters charOption = ThemeCharacters::borderHorizontal;
    bool editingChar = false;

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
                if (event.key.keysym.scancode == SDL_SCANCODE_1)
                {
                    state = ScreenState::colors;
                    console.setCurrentScreenIndex(colorScr);
                }
                else if (event.key.keysym.scancode == SDL_SCANCODE_2)
                {
                    state = ScreenState::characters;
                    console.setCurrentScreenIndex(charScr);
                }
                else if (event.key.keysym.scancode == SDL_SCANCODE_3)
                {
                    state = ScreenState::widgets;
                    console.setCurrentScreenIndex(widgetScr);
                }
                else if (state == ScreenState::colors)
                {
                    if (editingColor)
                    {
                        if (event.key.keysym.scancode == SDL_SCANCODE_UP)
                        {
                            rgbaSelection = std::clamp(rgbaSelection - 1, 0, 3);
                            colorR.lock()->setString(" " + std::to_string(getColor(theme, colorOption).r) + " ");
                            colorG.lock()->setString(" " + std::to_string(getColor(theme, colorOption).g) + " ");
                            colorB.lock()->setString(" " + std::to_string(getColor(theme, colorOption).b) + " ");
                            colorA.lock()->setString(" " + std::to_string(getColor(theme, colorOption).a) + " ");
                            switch (rgbaSelection)
                            {
                            case 0:
                                colorR.lock()->setString("<" + std::to_string(getColor(theme, colorOption).r) + ">");
                                break;
                            case 1:
                                colorG.lock()->setString("<" + std::to_string(getColor(theme, colorOption).g) + ">");
                                break;
                            case 2:
                                colorB.lock()->setString("<" + std::to_string(getColor(theme, colorOption).b) + ">");
                                break;
                            case 3: 
                                colorA.lock()->setString("<" + std::to_string(getColor(theme, colorOption).a) + ">");
                                break;
                            default:
                                break;
                            }
                        }
                        else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
                        {
                            rgbaSelection = std::clamp(rgbaSelection + 1, 0, 3);
                            colorR.lock()->setString(" " + std::to_string(getColor(theme, colorOption).r) + " ");
                            colorG.lock()->setString(" " + std::to_string(getColor(theme, colorOption).g) + " ");
                            colorB.lock()->setString(" " + std::to_string(getColor(theme, colorOption).b) + " ");
                            colorA.lock()->setString(" " + std::to_string(getColor(theme, colorOption).a) + " ");
                            switch (rgbaSelection)
                            {
                            case 0:
                                colorR.lock()->setString("<" + std::to_string(getColor(theme, colorOption).r) + ">");
                                break;
                            case 1:
                                colorG.lock()->setString("<" + std::to_string(getColor(theme, colorOption).g) + ">");
                                break;
                            case 2:
                                colorB.lock()->setString("<" + std::to_string(getColor(theme, colorOption).b) + ">");
                                break;
                            case 3: 
                                colorA.lock()->setString("<" + std::to_string(getColor(theme, colorOption).a) + ">");
                                break;
                            default:
                                break;
                            }
                        }
                        else if (event.key.keysym.scancode == SDL_SCANCODE_LEFT)
                        {
                            int32_t inc = -1;
                            if (event.key.keysym.mod == KMOD_LSHIFT) { inc = -10; }

                            switch (rgbaSelection)
                            {
                            case 0:
                               getColor(theme, colorOption).r = std::clamp(getColor(theme, colorOption).r + inc, 0, 255); 
                               colorR.lock()->setString("<" + std::to_string(getColor(theme, colorOption).r) + ">");
                               break;
                            case 1:
                               getColor(theme, colorOption).g = std::clamp(getColor(theme, colorOption).g + inc, 0, 255); 
                               colorG.lock()->setString("<" + std::to_string(getColor(theme, colorOption).g) + ">");
                               break;
                            case 2:
                               getColor(theme, colorOption).b = std::clamp(getColor(theme, colorOption).b + inc, 0, 255); 
                               colorB.lock()->setString("<" + std::to_string(getColor(theme, colorOption).b) + ">");
                               break;
                            case 3:
                               getColor(theme, colorOption).a = std::clamp(getColor(theme, colorOption).a + inc, 0, 255); 
                               colorA.lock()->setString("<" + std::to_string(getColor(theme, colorOption).a) + ">");
                               break;
                            default:
                               break;
                            }

                            console.setTheme(theme);
                        }
                        else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
                        {
                            int32_t inc = 1;
                            if (event.key.keysym.mod == KMOD_LSHIFT) { inc = 10; }

                            switch (rgbaSelection)
                            {
                            case 0:
                               getColor(theme, colorOption).r = std::clamp(getColor(theme, colorOption).r + inc, 0, 255); 
                               colorR.lock()->setString("<" + std::to_string(getColor(theme, colorOption).r) + ">");
                               break;
                            case 1:
                               getColor(theme, colorOption).g = std::clamp(getColor(theme, colorOption).g + inc, 0, 255); 
                               colorG.lock()->setString("<" + std::to_string(getColor(theme, colorOption).g) + ">");
                               break;
                            case 2:
                               getColor(theme, colorOption).b = std::clamp(getColor(theme, colorOption).b + inc, 0, 255); 
                               colorB.lock()->setString("<" + std::to_string(getColor(theme, colorOption).b) + ">");
                               break;
                            case 3:
                               getColor(theme, colorOption).a = std::clamp(getColor(theme, colorOption).a + inc, 0, 255); 
                               colorA.lock()->setString("<" + std::to_string(getColor(theme, colorOption).a) + ">");
                               break;
                            default:
                               break;
                            }

                            console.setTheme(theme);
                        }
                        else if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                        {
                            editingColor = false;
                            colorR.lock()->setString(" " + std::to_string(getColor(theme, colorOption).r) + " ");
                            colorG.lock()->setString(" " + std::to_string(getColor(theme, colorOption).g) + " ");
                            colorB.lock()->setString(" " + std::to_string(getColor(theme, colorOption).b) + " ");
                            colorA.lock()->setString(" " + std::to_string(getColor(theme, colorOption).a) + " ");
                            colorWm.activateWidget(colorList.lock()->getId());
                        }
                    }
                    else 
                    {
                        if (event.key.keysym.scancode == SDL_SCANCODE_UP)
                        {
                            colorList.lock()->scrollUp();
                        }
                        else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
                        {
                            colorList.lock()->scrollDown();
                        }
                        else if (event.key.keysym.scancode == SDL_SCANCODE_RETURN)
                        {
                            editingColor = true;
                            colorOption = colorList.lock()->getCurrentElement().mElement;
                            currentColor.lock()->setString(colorList.lock()->getCurrentElement().mName);
                            rgbaSelection = 0;
                            colorR.lock()->setString("<" + std::to_string(getColor(theme, colorOption).r) + ">");
                            colorG.lock()->setString(" " + std::to_string(getColor(theme, colorOption).g) + " ");
                            colorB.lock()->setString(" " + std::to_string(getColor(theme, colorOption).b) + " ");
                            colorA.lock()->setString(" " + std::to_string(getColor(theme, colorOption).a) + " ");
                            colorWm.deactivateWidget(colorList.lock()->getId());
                        }
                    }
                }
                else if (state == ScreenState::characters)
                {
                    if (editingChar)
                    {
                        if (event.key.keysym.scancode == SDL_SCANCODE_LEFT)
                        {
                            int32_t inc = -1;
                            if (event.key.keysym.mod == KMOD_LSHIFT) { inc = -10; }

                            getCharacter(theme, charOption) = std::clamp(getCharacter(theme, charOption) + inc, 0, 255);
                            charSlider.lock()->setString("<" + std::to_string(getCharacter(theme, charOption)) + ">");

                            console.setTheme(theme);
                        }
                        else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
                        {
                            int32_t inc = 1;
                            if (event.key.keysym.mod == KMOD_LSHIFT) { inc = 10; }

                            getCharacter(theme, charOption) = std::clamp(getCharacter(theme, charOption) + inc, 0, 255);
                            charSlider.lock()->setString("<" + std::to_string(getCharacter(theme, charOption)) + ">");

                            console.setTheme(theme);
                        }
                        else if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                        {
                            editingChar = false;
                            charSlider.lock()->setString(" " + std::to_string(getCharacter(theme, charOption)) + " ");
                            charWm.activateWidget(charList.lock()->getId());
                        }
                    }
                    else
                    {
                        if (event.key.keysym.scancode == SDL_SCANCODE_UP)
                        {
                            charList.lock()->scrollUp();
                        }
                        else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
                        {
                            charList.lock()->scrollDown();
                        }
                        else if (event.key.keysym.scancode == SDL_SCANCODE_RETURN)
                        {
                            editingChar = true;
                            charOption = charList.lock()->getCurrentElement().mElement;
                            currentChar.lock()->setString(charList.lock()->getCurrentElement().mName);
                            charSlider.lock()->setString("<" + std::to_string(getCharacter(theme, charOption)) + ">");
                            charWm.deactivateWidget(charList.lock()->getId());
                        }
                    }
                }
                else if (state == ScreenState::widgets)
                {
                    if (event.key.keysym.scancode == SDL_SCANCODE_UP)
                    {
                        wList.lock()->scrollUp();
                    }
                    else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
                    {
                        wList.lock()->scrollDown();
                    }
                    else if (event.key.keysym.scancode == SDL_SCANCODE_RETURN)
                    {
                        wList.lock()->toggleCurrentElement();
                    }
                }
            }

            console.doEvent(event);
        }

        console.render();
    }

    return 0;
}
