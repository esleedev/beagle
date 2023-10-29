#include "collisionTests.h"
#include "vector.h"
#include <iostream>

Vector2D GetPointClosestToPointInLine(Vector2D Point, Vector2D LinePointA, Vector2D LinePointB, float& Time)
{
	Vector2D point;

	Vector2D aToB = LinePointB - LinePointA;
	Time = GetDotProduct(Point - LinePointA, aToB) / GetDotProduct(aToB, aToB);

	return LinePointA + Clamp(Time, 0, 1) * aToB;
}

bool IsLineIntersectingRectangle(Vector2D LinePointA, Vector2D LinePointB, Rectangle Rectangle, LineHitRectangle& Hits)
{
	// get sides of the rectangle
	Vector2D sidePoints[8] =
	{
		{ Rectangle.x + Rectangle.width, Rectangle.y },  { Rectangle.x + Rectangle.width, Rectangle.y + Rectangle.height },
		{ Rectangle.x, Rectangle.y },                    { Rectangle.x + Rectangle.width, Rectangle.y },
		{ Rectangle.x, Rectangle.y },                    { Rectangle.x,                   Rectangle.y + Rectangle.height },
		{ Rectangle.x, Rectangle.y + Rectangle.height }, { Rectangle.x + Rectangle.width, Rectangle.y + Rectangle.height }
	};

	Hits = {};
	for (int side = 0; side < 4; side++)
	{
		Vector2D intersectionPoint;
		if (IsLineIntersectingLine(LinePointA, LinePointB, sidePoints[side * 2], sidePoints[side * 2 + 1], intersectionPoint))
		{
			Hits.hasHitSide[side] = true;
			Hits.hitPoints[side] = intersectionPoint;
		}
		else
		{
			Hits.hasHitSide[side] = false;
			Hits.hitPoints[side] = { 0, 0 };
		}
	}

	return Hits.hasHitSide[0] || Hits.hasHitSide[1] || Hits.hasHitSide[2] || Hits.hasHitSide[3];
}

bool IsLineIntersectingLine(Vector2D LineAPointOne, Vector2D LineAPointTwo, Vector2D LineBPointOne, Vector2D LineBPointTwo, Vector2D& Intersection)
{
	float x1 = LineAPointOne.x;
	float x2 = LineAPointTwo.x;
	float x3 = LineBPointOne.x;
	float x4 = LineBPointTwo.x;
	float y1 = LineAPointOne.y;
	float y2 = LineAPointTwo.y;
	float y3 = LineBPointOne.y;
	float y4 = LineBPointTwo.y;

	float timeA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	float timeB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

	if (timeA >= 0 && timeA <= 1.0 && timeB >= 0 && timeB <= 1.0)
	{
		Intersection = { x1 + timeA * (x2 - x1), y1 + timeA * (y2 - y1) };
		return true;
	}

	return false;
}

bool IsPointInRectangle(Vector2D Point, Rectangle Rectangle)
{
	return !(Point.x < Rectangle.x || Point.y < Rectangle.y || Point.x > Rectangle.x + Rectangle.width || Point.y > Rectangle.y + Rectangle.height);
}

