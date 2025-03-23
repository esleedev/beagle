#pragma once

#include "resource_types.h"

namespace esl_internal
{
	void UpdateLines(std::shared_ptr<esl::Resources> Resources);
	void UpdateTexts(std::shared_ptr<esl::Resources> Resources);

	// takes a string and adds line breaks based on font glyphs and horizontal size to prevent overflow
	void WrapTextHorizontally(std::string& TextString, std::shared_ptr<esl::Font> Font, float HorizontalSize);
	// gets the actual mesh width of each line in the string, based on line breaks in string and the size of font glyphs
	void GetLineWidthsFromTextString(const std::string& TextString, std::shared_ptr<esl::Font> Font, std::vector<float>& LineWidths);
}