#pragma once

#include "vector.h"
#include "matrix.h"

struct Transform
{
	Vector position;
	Vector scale;
	float yaw;
	Matrix matrix;
	bool shouldUpdateMatrix;

	Transform();
	void UpdateMatrix();
	void SetPosition(Vector Position);
	void SetPosition(Vector2D Position);
	void SetPositionAndScale(Vector Position, Vector Scale);
};