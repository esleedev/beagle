#include "graphicsTypes.h"
#include "piece.h"

DynamicMesh::DynamicMesh(Mesh* Mesh)
{
	mesh = Mesh;
}

void PuzzleMesh::UpdateVertices()
{
	vertices.clear();
	for (int piece = 0; piece < pieces.size(); piece++)
	{
		Vertex pieceVertices[4];
		Vector2D origin = pieces[piece].origin;
		Vector2D size = pieces[piece].size;
		Vector2D position = pieces[piece].position;
		pieceVertices[0].position = { position.x - origin.x, position.y - (size.y - origin.y), 0 };
		pieceVertices[1].position = { position.x + size.x - origin.x, position.y - (size.y - origin.y), 0 };
		pieceVertices[2].position = { position.x - origin.x, position.y + size.y - (size.y - origin.y), 0 };
		pieceVertices[3].position = { position.x + size.x - origin.x, position.y + size.y - (size.y - origin.y), 0 };
		for (int pieceVertex = 0; pieceVertex < 4; pieceVertex++)
			vertices.push_back(pieceVertices[pieceVertex]);
	}
}