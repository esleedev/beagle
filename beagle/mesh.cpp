#include "mesh.h"

Mesh GenerateQuad
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
    GLuint indexData[] = { 0, 2, 1, 1, 2, 3 };

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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indexData, GL_STATIC_DRAW);

    return mesh;
}

Mesh GenerateLine(Line2D Line)
{
    Mesh mesh = {};
    Vector2D bToA = Line.pointA - Line.pointB;
    bToA.Normalize();
    Vector right = { bToA.y, -bToA.x, 0 }; // rotate vector clockwise
    float halfWidth = (Line.width * 0.5);

    Vertex vertices[] =
    {
        Vertex{ Vector{ Line.pointA.x, Line.pointA.y, 0 } - right * halfWidth, { 0, 1 } },
        Vertex{ Vector{ Line.pointA.x, Line.pointA.y, 0 } + right * halfWidth, { 1, 1 } },
        Vertex{ Vector{ Line.pointB.x, Line.pointB.y, 0 } - right * halfWidth, { 0, 0 } },
        Vertex{ Vector{ Line.pointB.x, Line.pointB.y, 0 } + right * halfWidth, { 1, 0 } }
    };
    GLuint indexData[] = { 0, 1, 2, 1, 3, 2 };

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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indexData, GL_STATIC_DRAW);

    return mesh;
}