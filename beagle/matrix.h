#pragma once

struct Vector;

struct Matrix
{
	float matrix[4][4];

	void SetIdentity();
	void SetTranslation(Vector Position);
	void SetRotation(float YawInDegrees);
	void SetScale(Vector Scale);
	void SetTranslationAndRotationAndScale(Vector Position, float YawInDegrees, Vector Scale);

    Matrix operator*(const Matrix& OtherMatrix);
};