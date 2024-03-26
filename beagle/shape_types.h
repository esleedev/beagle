#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace esl
{
	struct Sphere
	{
		glm::vec3 position;
		float radius;
	};

	struct Line
	{
		glm::vec3 pointA;
		glm::vec3 pointB;
	};
}