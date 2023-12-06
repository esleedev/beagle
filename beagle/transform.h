#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/quaternion_float.hpp>

namespace esl
{
	struct Transform
	{
		glm::vec3 position;
		float yaw, pitch;

		glm::quat rotation;
		glm::mat4x4 matrix;

		void CalculateRotation();
	};
}