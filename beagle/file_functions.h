#pragma once

#include <string>
#include <vector>

namespace esl
{
	std::string GetFileSource(std::string FilePath);
	void GetFileLineByLine(std::string FilePath, std::vector<std::string>& Lines);
	void FindValuesInLine(std::string Line, std::vector<std::string>& Values, char Separator);
}