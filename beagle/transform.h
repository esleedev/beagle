#pragma once

#include "vector.h"
#include "matrix.h"

struct Transform
{
	Vector position;
	float yaw;
	Matrix matrix;
	bool shouldUpdateMatrix;

	Transform();
	void UpdateMatrix();
	void SetPosition(Vector Position);
	void SetPosition(Vector2D Position);
};