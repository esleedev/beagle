#pragma once

struct Vector;

struct Matrix
{
	float matrix[4][4];

	void SetIdentity();
	void Move(Vector Vector);
};