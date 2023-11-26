#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace esl
{
	struct Transform
	{
		glm::vec3 position;
		glm::mat4x4 matrix;
	};
}