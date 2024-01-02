#include <gl/glew.h>
#include <fstream>

#include "mesh_functions.h"
#include "file_functions.h"
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
    mesh.indices = { 0, 3, 2, 0, 1, 3 };

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

void esl::LoadOBJ(std::string FilePath, std::vector<esl::Vertex>& Vertices, std::vector<esl::uint>& Indices)
{
    std::vector<std::string> lines;

    std::vector<glm::vec3> sourceVertices;
    std::vector<glm::vec2> sourceUVs;
    std::vector<esl::uint> sourceIndices;
    std::vector<esl::uint> sourceUVIndices;

    esl::GetFileLineByLine(FilePath, lines);
    for (int line = 0; line < lines.size(); line++)
    {
        std::vector<std::string> values;
        esl::FindValuesInLine(lines[line], values, ' ');
        if (values.size() == 0) continue;
        if (values[0] == "v")
            sourceVertices.push_back({ std::stof(values[1]), std::stof(values[2]), std::stof(values[3]) });
        else if (values[0] == "vt")
            sourceUVs.push_back({ std::stof(values[1]), 1.0f - std::stof(values[2]) });
        else if (values[0] == "f")
        {
            for (int index = 0; index < 3; index++)
            {
                std::vector<std::string> indexValues;
                esl::FindValuesInLine(values[1 + index], indexValues, '/');
                
                sourceIndices.push_back(std::stoi(indexValues[0]) - 1);
                sourceUVIndices.push_back(std::stoi(indexValues[1]) - 1);
            }
        }
    }

    for (esl::uint index = 0; index < sourceIndices.size(); index++)
    {
        Vertices.push_back(esl::Vertex(sourceVertices[sourceIndices[index]], sourceUVs[sourceUVIndices[index]]));
        Indices.push_back(index);
    }
}

esl::Mesh esl::LoadOBJAsMesh(std::string FilePath)
{
    return esl::Mesh();
}

