#pragma once

#include <memory>

#include "resource_types.h"

namespace esl
{
	class SpriteSystem
	{
	public:
		void UpdateSprites(std::shared_ptr<esl::Resources> Resources, float DeltaTime);
	};
}