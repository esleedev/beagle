#pragma once

struct Vector2D
{
	float x, y;

	Vector2D operator-();
};

struct Vector
{
	float x, y, z;

	Vector operator-();
	Vector2D GetXY();

	void Set(float X, float Y, float Z);
	void Set(Vector2D XY);
};

Vector2D operator+(Vector2D A, Vector2D B);
Vector2D operator-(Vector2D A, Vector2D B);
Vector2D operator*(Vector2D Point, float Scalar);
Vector2D operator*(float Scalar, Vector2D Point);

float GetDotProduct(Vector2D A, Vector2D B);
float GetDotProduct(Vector A, Vector B);

struct Rectangle
{
	float x, y, width, height;
};

float Clamp(float Value, float Minimum, float Maximum);