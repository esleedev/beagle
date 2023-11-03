#include "matrix.h"
#include "vector.h"
#include <cmath>

void Matrix::SetIdentity()
{
	matrix[0][0] = 1.0f; matrix[0][1] = 0.0f; matrix[0][2] = 0.0f; matrix[0][3] = 0.0f;
	matrix[1][0] = 0.0f; matrix[1][1] = 1.0f; matrix[1][2] = 0.0f; matrix[1][3] = 0.0f;
	matrix[2][0] = 0.0f; matrix[2][1] = 0.0f; matrix[2][2] = 1.0f; matrix[2][3] = 0.0f;
	matrix[3][0] = 0.0f; matrix[3][1] = 0.0f; matrix[3][2] = 0.0f; matrix[3][3] = 1.0f;
}

void Matrix::SetTranslation(Vector Vector)
{
	SetIdentity();
	matrix[0][3] = Vector.x;
	matrix[1][3] = Vector.y;
	matrix[2][3] = Vector.z;
}

void Matrix::SetRotation(float YawInDegrees)
{
	SetIdentity();

	float cosOfAngle = cos(YawInDegrees * 0.0174532925199f);
	float sinOfAngle = sin(YawInDegrees * 0.0174532925199f);

	matrix[0][0] = cosOfAngle;
	matrix[0][2] = -sinOfAngle;
	matrix[2][0] = sinOfAngle;
	matrix[2][2] = cosOfAngle;
}

void Matrix::SetScale(Vector Scale)
{
	SetIdentity();
	matrix[0][0] = Scale.x;
	matrix[1][1] = Scale.y;
	matrix[2][2] = Scale.z;
}

void Matrix::SetTranslationAndRotationAndScale
(
	Vector Position,
	float YawInDegrees,
	Vector Scale
)
{
	Matrix translationMatrix, rotationMatrix, scaleMatrix;
	translationMatrix.SetTranslation(Position);
	rotationMatrix.SetRotation(YawInDegrees);
	scaleMatrix.SetScale(Scale);
	*this = translationMatrix * scaleMatrix * rotationMatrix;
}

Matrix Matrix::operator*(const Matrix& OtherMatrix)
{
	Matrix newMatrix = {};
    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {
			newMatrix.matrix[x][y] =
			(
				matrix[x][0] * OtherMatrix.matrix[0][y] +
				matrix[x][1] * OtherMatrix.matrix[1][y] +
				matrix[x][2] * OtherMatrix.matrix[2][y] +
				matrix[x][3] * OtherMatrix.matrix[3][y]
			);
        }
    }
    return newMatrix;
}