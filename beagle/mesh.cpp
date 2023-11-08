#include "mesh.h"

Mesh GenerateQuadObject
(
    Vector2D Size, Vector2D Origin
)
{
    Mesh mesh = {};

    Vertex vertices[] =
    {
        Vertex{ { 0 - Origin.x,      0 - Origin.y,      0 }, { 0, 1 } },
        Vertex{ { Size.x - Origin.x, 0 - Origin.y,      0 }, { 1, 1 } },
        Vertex{ { 0 - Origin.x,      Size.y - Origin.y, 0 }, { 0, 0 } },
        Vertex{ { Size.x - Origin.x, Size.y - Origin.y, 0 }, { 1, 0 } }
    };
    GLuint indices[] = { 0, 2, 1, 1, 2, 3 };
    mesh.indexCount = 6;

    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(PositionAttribute);
    glEnableVertexAttribArray(UVAttribute);
    glVertexAttribPointer(PositionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(UVAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);

    glGenBuffers(1, &mesh.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);

    return mesh;
}

Mesh GenerateEmptyMeshObject()
{
    Mesh mesh = {};

    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

    glEnableVertexAttribArray(PositionAttribute);
    glEnableVertexAttribArray(UVAttribute);
    glVertexAttribPointer(PositionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(UVAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);

    glGenBuffers(1, &mesh.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

    return mesh;
}

void AddLineToMesh(Line2D Line, DynamicMesh* Mesh)
{
    // get offset to create width
    Vector2D bToA = Line.pointA - Line.pointB;
    bToA.Normalize();
    Vector right = { bToA.y, -bToA.x, 0 }; // rotate vector clockwise
    float halfWidth = (Line.width * 0.5);

    // make line

    Vertex vertices[] =
    {
        Vertex{ Vector{ Line.pointA.x, Line.pointA.y, 0 } - right * halfWidth, { 0, 1 } },
        Vertex{ Vector{ Line.pointA.x, Line.pointA.y, 0 } + right * halfWidth, { 0, 0 } },
        Vertex{ Vector{ Line.pointB.x, Line.pointB.y, 0 } - right * halfWidth, { 1, 1 } },
        Vertex{ Vector{ Line.pointB.x, Line.pointB.y, 0 } + right * halfWidth, { 1, 0 } }
    };
    GLuint indices[] = { 0, 1, 2, 1, 3, 2 };

    // add line to mesh
    int lastVertexCount = Mesh->vertices.size();
    for (int index = 0; index < 6; index++)
    {
        Mesh->indices.push_back(indices[index] + lastVertexCount);
    }
    for (int vertex = 0; vertex < 4; vertex++)
    {
        Mesh->vertices.push_back(vertices[vertex]);
    }
}

void AddPieceToMesh(Piece Piece, DynamicMesh* Mesh)
{
    Vector2D origin = Piece.origin;
    Vector2D size = Piece.size;
    Vector2D position = Piece.position;
    Vertex vertices[4];
    vertices[0].position = { position.x - origin.x, position.y - (size.y - origin.y), 0 };
    vertices[1].position = { position.x + size.x - origin.x, position.y - (size.y - origin.y), 0 };
    vertices[2].position = { position.x - origin.x, position.y + size.y - (size.y - origin.y), 0 };
    vertices[3].position = { position.x + size.x - origin.x, position.y + size.y - (size.y - origin.y), 0 };
    Rectangle uvs = Piece.tile.GetUVs();
    vertices[0].uv = { uvs.x,             uvs.y + uvs.height };
    vertices[1].uv = { uvs.x + uvs.width, uvs.y + uvs.height };
    vertices[2].uv = { uvs.x,             uvs.y };
    vertices[3].uv = { uvs.x + uvs.width, uvs.y };

    GLuint indices[] = { 0, 1, 2, 1, 3, 2 };

    // add line to mesh
    int lastVertexCount = Mesh->vertices.size();
    for (int index = 0; index < 6; index++)
    {
        Mesh->indices.push_back(indices[index] + lastVertexCount);
    }
    for (int vertex = 0; vertex < 4; vertex++)
    {
        Mesh->vertices.push_back(vertices[vertex]);
    }
}