#pragma once

struct Vector;

struct Matrix
{
	float matrix[4][4];

	void SetIdentity();
	void SetTranslation(Vector Position);
	void SetRotation(float YawInDegrees);
	void SetTranslationAndRotation(Vector Position, float YawInDegrees);

    Matrix operator*(const Matrix& OtherMatrix);
};