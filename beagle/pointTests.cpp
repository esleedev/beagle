#include "pointTests.h"
#include "vector.h"

Vector2D GetPointClosestToPointInLine(Vector2D Point, Vector2D LinePointA, Vector2D LinePointB, float& Time)
{
	Vector2D point;

	Vector2D aToB = LinePointB - LinePointA;
	Time = GetDotProduct(Point - LinePointA, aToB) / GetDotProduct(aToB, aToB);

	return LinePointA + Clamp(Time, 0, 1) * aToB;
}