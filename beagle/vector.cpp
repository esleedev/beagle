#include "vector.h"

Vector2D Vector::GetXY()
{
	return Vector2D { x, y };
}

void Vector::Set(float X, float Y, float Z)
{
	x = X; y = Y; z = Z;
}

void Vector::Set(Vector2D XY)
{
	x = XY.x;
	y = XY.y;
}

Vector Vector::operator-()
{
	Vector vector = *this;
	vector.x = -vector.x;
	vector.y = -vector.y;
	vector.z = -vector.z;
	return vector;
}

Vector2D Vector2D::operator-()
{
	Vector2D vector = *this;
	vector.x = -vector.x;
	vector.y = -vector.y;
	return vector;
}

Vector2D operator+(Vector2D A, Vector2D B)
{
	return Vector2D{ A.x + B.x, A.y + B.y };
}

Vector2D operator-(Vector2D A, Vector2D B)
{
	return Vector2D{ A.x - B.x, A.y - B.y };
}

Vector2D operator*(Vector2D Point, float Scalar)
{
	return Vector2D{ Point.x * Scalar, Point.y * Scalar };
}

Vector2D operator*(float Scalar, Vector2D Point)
{
	return Vector2D{ Point.x * Scalar, Point.y * Scalar };
}

float GetDotProduct(Vector2D A, Vector2D B)
{
	return A.x * B.x + A.y * B.y;
}

float GetDotProduct(Vector A, Vector B)
{
	return A.x * B.x + A.y * B.y + A.z * B.z;
}

float Clamp(float Value, float Minimum, float Maximum)
{
	return Value < Minimum ? Minimum : (Value > Maximum) ? Maximum : Value;
}