#pragma once

struct Vector;
struct Matrix;

struct Transform
{
	Vector position;
	float yaw;
	Matrix matrix;

	void UpdateMatrix();
};