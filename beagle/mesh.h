#pragma once

#include "graphicsTypes.h"

const int PositionAttribute = 0;
const int UVAttribute = 1;

Mesh GenerateQuad(Vector2D Size = {}, Vector2D Origin = {});
void AddLineToMesh(Line2D Line, DynamicMesh* Mesh);
Mesh GenerateEmptyMesh();