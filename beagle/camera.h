#pragma once

#include <glm/vec3.hpp>

namespace esl
{
	class Camera
	{
	public:
		// projection settings
		float aspectRatio, verticalFieldOfViewInDegrees, near, far;

		glm::vec3 position, lookAt;

		void SetProjectionSettings(float AspectRatio, float VerticalFieldOfViewInDegrees, float Near, float Far);
		void SetViewSettings(glm::vec3 Position, glm::vec3 LookAt);
	};
}