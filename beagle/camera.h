#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace esl
{
	class Camera
	{
	public:
		// projection settings
		float aspectRatio, verticalFieldOfViewInDegrees, near, far;

		glm::vec3 position;
		float yaw, pitch;

		glm::mat4x4 projectionMatrix, viewMatrix;

		void SetProjectionSettings(float AspectRatio, float VerticalFieldOfViewInDegrees, float Near, float Far);
		void SetViewSettings(glm::vec3 Position, float Yaw, float Pitch);
	};
}