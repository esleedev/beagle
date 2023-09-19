#pragma once

#include "vector.h"
#include "matrix.h"

struct Transform
{
	Vector position;
	float yaw;
	Matrix matrix;
	bool shouldUpdateMatrix;

	void UpdateMatrix();
};