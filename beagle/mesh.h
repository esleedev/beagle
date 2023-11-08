#pragma once

#include "graphicsTypes.h"
#include "piece.h"

const int PositionAttribute = 0;
const int UVAttribute = 1;

Mesh GenerateQuadObject(Vector2D Size = {}, Vector2D Origin = {});
Mesh GenerateEmptyMeshObject();
void AddLineToMesh(Line2D Line, DynamicMesh* Mesh);
void AddLinesToMesh(std::vector<Line2D> Lines, DynamicMesh* Mesh);
void AddPieceToMesh(Piece Piece, DynamicMesh* Mesh);
