#include "transform.h"
#include "vector.h"
#include "matrix.h"

Transform::Transform()
{
	position = { 0, 0, 0 };
	scale = { 1, 1, 1 };
	yaw = 0;
	matrix.SetIdentity();
	shouldUpdateMatrix = false;
}

void Transform::UpdateMatrix()
{
	matrix.SetTranslationAndRotationAndScale(position, yaw, scale);
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

void Transform::SetPositionAndScale(Vector Position, Vector Scale)
{
	position = Position;
	scale = Scale;
	shouldUpdateMatrix = true;
}
