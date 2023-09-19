#pragma once

struct Vector;

struct Matrix
{
	float matrix[4][4];

	void SetIdentity();
	void SetTranslation(Vector Vector);
	void SetRotation(float YawInDegrees);

    Matrix operator*(const Matrix& OtherMatrix);
};