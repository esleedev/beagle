#pragma once

#include "graphicsTypes.h"

struct Piece
{
	Tile tile;
	Vector2D position;
	Vector2D size;
	Vector2D origin;
};

// tilesizex, tilesizey, tileid, x, y, sizex, sizey, originx, originy