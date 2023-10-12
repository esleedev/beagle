#pragma once

#include "vector.h"

struct LineHitRectangle
{
	Vector2D hitPoints[4];
	bool hasHitSide[4];
};

enum CardinalDirections
{
	Right = 0,
	Bottom = 1,
	Left = 2,
	Top = 3
};

Vector2D GetPointClosestToPointInLine(Vector2D Point, Vector2D LinePointA, Vector2D LinePointB, float& Time);
bool IsLineIntersectingRectangle(Vector2D LinePointA, Vector2D LinePointB, Rectangle Rectangle, LineHitRectangle& Hits);
bool IsLineIntersectingLine(Vector2D LineAPointA, Vector2D LineAPointB, Vector2D LineBPointA, Vector2D LineBPointB, Vector2D& Intersection);

