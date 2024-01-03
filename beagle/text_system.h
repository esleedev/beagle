#pragma once

#include <memory>

#include "resource_types.h"

namespace esl
{
	class TextSystem
	{
	public:
		void UpdateTexts(std::shared_ptr<esl::Resources> Resources);
	};
}