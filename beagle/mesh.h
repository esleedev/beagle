#pragma once

#include "graphicsTypes.h"
#include "piece.h"

const int PositionAttribute = 0;
const int UVAttribute = 1;

Mesh* GenerateNewQuad(Vector2D Size = {}, Vector2D Origin = {});
void AddLineToMesh(Line2D Line, DynamicMesh* Mesh);
void AddPieceToMesh(Piece Piece, DynamicMesh* Mesh);
Mesh* GenerateNewEmptyMesh();