#pragma once

#include "../resource_types.h"

namespace game
{
	class PlayerSystem : public esl::System
	{
		glm::vec2 velocity;
	public:
		void Start(std::shared_ptr<esl::Resources> Resources);
		void Update(float DeltaTime, esl::Input* const Input, std::shared_ptr<esl::Resources> Resources);
	};
}