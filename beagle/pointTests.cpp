#include "pointTests.h"
#include "vector.h"

Vector2D GetPointClosestToPointInLine(Vector2D Point, Vector2D LinePointA, Vector2D LinePointB)
{
	Vector2D point;

	Vector2D aToB = LinePointB - LinePointA;
	float time = GetDotProduct(Point - LinePointA, aToB) / GetDotProduct(aToB, aToB);
	time = Clamp(time, 0, 1);

	return LinePointA + time * aToB;
}