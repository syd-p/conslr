#include "theme.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

conslr::Theme::Theme(const std::string& file)
{
    std::ifstream ifs(file);
    if (!ifs.good())
    {
        ifs.close();
        std::cerr << "Failed to open file: " << file << std::endl;
        return;
    }

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
                borderHorizontal = value.at(1);
                continue;
            }

            borderHorizontal = std::stoi(value);
            continue;
        }

        if (param == "borderVertical")
        {
            if (value.at(0) == '\'')
            {
                borderVertical = value.at(1);
                continue;
            }

            borderVertical = std::stoi(value);
            continue;
        }

        if (param == "borderCornerTl")
        {
            if (value.at(0) == '\'')
            {
                borderCornerTl = value.at(1);
                continue;
            }

            borderCornerTl = std::stoi(value);
            continue;
        }

        if (param == "borderCornerTr")
        {
            if (value.at(0) == '\'')
            {
                borderCornerTr = value.at(1);
                continue;
            }

            borderCornerTr = std::stoi(value);
            continue;
        }

        if (param == "borderCornerBl")
        {
            if (value.at(0) == '\'')
            {
                borderCornerBl= value.at(1);
                continue;
            }

            borderCornerBl = std::stoi(value);
            continue;
        }

        if (param == "borderCornerBr")
        {
            if (value.at(0) == '\'')
            {
                borderCornerBr = value.at(1);
                continue;
            }

            borderCornerBr = std::stoi(value);
            continue;
        }

        if (param == "scrollbarCharacter")
        {
            if (value.at(0) == '\'')
            {
                scrollbarCharacter = value.at(1);
                continue;
            }

            scrollbarCharacter = std::stoi(value);
            continue;
        }

        if (param == "scrollbarCharacterHorizontal")
        {
            if (value.at(0) == '\'')
            {
                scrollbarCharacterHorizontal = value.at(1);
                continue;
            }

            scrollbarCharacterHorizontal = std::stoi(value);
            continue;
        }

        if (param == "selectionTick")
        {
            if (value.at(0) == '\'')
            {
                selectionTick = value.at(1);
                continue;
            }

            selectionTick = std::stoi(value);
            continue;
        }

        if (param == "progress")
        {
            if (value.at(0) == '\'')
            {
                progress = value.at(1);
                continue;
            }

            progress = std::stoi(value);
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
            
            background = { (uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)a };
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
            
            text = { (uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)a };
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
            
            border = { (uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)a };
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
            
            selection = { (uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)a };
            continue;
        }
    }

    ifs.close();
    return;
}
