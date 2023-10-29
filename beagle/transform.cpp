#include "transform.h"
#include "vector.h"
#include "matrix.h"

Transform::Transform()
{
	position = { 0, 0, 0 };
	yaw = 0;
	matrix.SetIdentity();
	shouldUpdateMatrix = false;
}

void Transform::UpdateMatrix()
{
	matrix.SetTranslationAndRotation(position, yaw);
}

void Transform::SetPosition(Vector Position)
{
	position = Position;
	shouldUpdateMatrix = true;
}

void Transform::SetPosition(Vector2D Position)
{
	position.x = Position.x;
	position.y = Position.y;
	shouldUpdateMatrix = true;
}