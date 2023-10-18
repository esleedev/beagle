#pragma once

#include <string>
#include <vector>

void FindValuesInLine(std::string Line, std::vector<std::string>& Values)
{
    size_t position = 0;
    while (position < Line.size())
    {
        if (Line[position] == ' ' || Line[position] == ',')
        {
            position++;
            continue;
        }

        size_t findComma = Line.find(',', position);
        if (findComma != std::string::npos)
        {
            Values.push_back(Line.substr(position, findComma - position));
        }
        else
        {
            Values.push_back(Line.substr(position));
        }
        position = findComma;
    }
}