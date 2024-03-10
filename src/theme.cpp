#include "theme.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

conslr::Theme conslr::loadThemeFromFile(const std::string& file)
{
    std::ifstream ifs(file);
    if (!ifs.good())
    {
        ifs.close();
        std::cerr << "Failed to open file: " << file << std::endl;
        return conslr::Theme{};
    }

    conslr::Theme theme{};
    std::string line;
    while (std::getline(ifs, line))
    {
        std::stringstream ss{line};
        std::string param;
        std::string value;

        std::getline(ss, param, '=');
        std::getline(ss, value);

        //Process params and values
        if (param == "borderHorizontal")
        {
            if (value.at(0) == '\'')
            {
                theme.borderHorizontal = value.at(1);
                continue;
            }

            theme.borderHorizontal = std::stoi(value);
            continue;
        }

        if (param == "borderVertical")
        {
            if (value.at(0) == '\'')
            {
                theme.borderVertical = value.at(1);
                continue;
            }

            theme.borderVertical = std::stoi(value);
            continue;
        }

        if (param == "borderCornerTl")
        {
            if (value.at(0) == '\'')
            {
                theme.borderCornerTl = value.at(1);
                continue;
            }

            theme.borderCornerTl = std::stoi(value);
            continue;
        }

        if (param == "borderCornerTr")
        {
            if (value.at(0) == '\'')
            {
                theme.borderCornerTr = value.at(1);
                continue;
            }

            theme.borderCornerTr = std::stoi(value);
            continue;
        }

        if (param == "borderCornerBl")
        {
            if (value.at(0) == '\'')
            {
                theme.borderCornerBl= value.at(1);
                continue;
            }

            theme.borderCornerBl = std::stoi(value);
            continue;
        }

        if (param == "borderCornerBr")
        {
            if (value.at(0) == '\'')
            {
                theme.borderCornerBr = value.at(1);
                continue;
            }

            theme.borderCornerBr = std::stoi(value);
            continue;
        }

        if (param == "scrollbarCharacter")
        {
            if (value.at(0) == '\'')
            {
                theme.scrollbarCharacter = value.at(1);
                continue;
            }

            theme.scrollbarCharacter = std::stoi(value);
            continue;
        }

        if (param == "scrollbarCharacterHorizontal")
        {
            if (value.at(0) == '\'')
            {
                theme.scrollbarCharacterHorizontal = value.at(1);
                continue;
            }

            theme.scrollbarCharacterHorizontal = std::stoi(value);
            continue;
        }

        if (param == "selectionTick")
        {
            if (value.at(0) == '\'')
            {
                theme.selectionTick = value.at(1);
                continue;
            }

            theme.selectionTick = std::stoi(value);
            continue;
        }

        if (param == "progress")
        {
            if (value.at(0) == '\'')
            {
                theme.progress = value.at(1);
                continue;
            }

            theme.progress = std::stoi(value);
            continue;
        }

        if (param == "background")
        {
            int32_t r;
            int32_t g;
            int32_t b;
            int32_t a;
            
            std::stringstream colors{value};
            colors >> r >> g >> b >> a;
            
            theme.background = { (uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)a };
            continue;
        }

        if (param == "text")
        {
            int32_t r;
            int32_t g;
            int32_t b;
            int32_t a;
            
            std::stringstream colors{value};
            colors >> r >> g >> b >> a;
            
            theme.text = { (uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)a };
            continue;
        }

        if (param == "border")
        {
            int32_t r;
            int32_t g;
            int32_t b;
            int32_t a;
            
            std::stringstream colors{value};
            colors >> r >> g >> b >> a;
            
            theme.border = { (uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)a };
            continue;
        }

        if (param == "selection")
        {
            int32_t r;
            int32_t g;
            int32_t b;
            int32_t a;
            
            std::stringstream colors{value};
            colors >> r >> g >> b >> a;
            
            theme.selection = { (uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)a };
            continue;
        }
    }

    ifs.close();
    return theme;
}
