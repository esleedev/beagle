#include <fstream>
#include <iostream>

#include "file_functions.h"

std::string esl::GetFileSource(std::string FilePath)
{
    std::string line, lines;
    std::ifstream file;
    file.open(FilePath);
    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            lines += line + "\n";
        }
        file.close();
    }
    else
    {
        std::cout << "File not found: " << FilePath << std::endl;
    }

    return lines;
}

void esl::GetFileLineByLine(std::string FilePath, std::vector<std::string>& Lines)
{
    std::string line;
    std::ifstream file;
    file.open(FilePath);
    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            Lines.push_back(line);
        }
        file.close();
    }
    else
    {
        std::cout << "File not found: " << FilePath << std::endl;
    }
}

void esl::FindValuesInLine(std::string Line, std::vector<std::string>& Values, char Separator)
{
    size_t position = 0;
    while (position < Line.size())
    {
        if (Line[position] == ' ' || Line[position] == Separator)
        {
            position++;
            continue;
        }

        size_t separatorPosition = Line.find(Separator, position);
        if (separatorPosition != std::string::npos)
        {
            Values.push_back(Line.substr(position, separatorPosition - position));
        }
        else
        {
            Values.push_back(Line.substr(position));
        }
        position = separatorPosition;
    }
}
