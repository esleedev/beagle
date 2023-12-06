#include "camera.h"

void esl::Camera::SetProjectionSettings(float AspectRatio, float VerticalFieldOfViewInDegrees, float Near, float Far)
{
	aspectRatio = AspectRatio;
	verticalFieldOfViewInDegrees = VerticalFieldOfViewInDegrees;
	near = Near;
	far = Far;
}

void esl::Camera::SetViewSettings(glm::vec3 Position, float Yaw, float Pitch)
{
	position = Position;
	yaw = Yaw;
	pitch = Pitch;
}
