#pragma once

#include <memory>

#include "resource_types.h"

namespace esl
{
	class RenderSystem
	{
	public:
		void RenderObjects(std::shared_ptr<esl::Resources> Resources);
	};
}