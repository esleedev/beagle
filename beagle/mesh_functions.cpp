#include <gl/glew.h>

#include "mesh_functions.h"
#include "vertex.h"

esl::Mesh esl::GenerateQuadMesh(glm::vec2 Size, glm::vec2 Origin)
{
    esl::Mesh mesh = {};

    mesh.vertices =
    {
        esl::Vertex(glm::vec3(0.0f - Origin.x,   0.0f - Origin.y,   0.0f), glm::vec2( 0.0f, 1.0f )),
        esl::Vertex(glm::vec3(Size.x - Origin.x, 0.0f - Origin.y,   0.0f), glm::vec2( 1.0f, 1.0f )),
        esl::Vertex(glm::vec3(0.0f - Origin.x,   Size.y - Origin.y, 0.0f), glm::vec2( 0.0f, 0.0f )),
        esl::Vertex(glm::vec3(Size.x - Origin.x, Size.y - Origin.y, 0.0f), glm::vec2( 1.0f, 0.0f ))
    };
    mesh.indices = { 0, 2, 1, 1, 2, 3 };

    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(esl::Vertex), &mesh.vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(esl::PositionAttribute);
    glEnableVertexAttribArray(esl::UVAttribute);
    glVertexAttribPointer(esl::PositionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(esl::Vertex), 0);
    glVertexAttribPointer(esl::UVAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(esl::Vertex), (const GLvoid*)12);

    glGenBuffers(1, &mesh.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(esl::uint), &mesh.indices[0], GL_STATIC_DRAW);

    return mesh;
}

void esl::DeleteMeshes(std::vector<esl::Mesh> Meshes)
{
    for (int mesh = 0; mesh < Meshes.size(); mesh++)
    {
        glDeleteVertexArrays(1, &Meshes[mesh].vao);
        glDeleteBuffers(1, &Meshes[mesh].vbo);
        glDeleteBuffers(1, &Meshes[mesh].ibo);
    }
}

