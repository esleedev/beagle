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
    glEnableVertexAttribArray(esl::ColorAttribute);
    glVertexAttribPointer(esl::PositionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(esl::Vertex), 0);
    glVertexAttribPointer(esl::UVAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(esl::Vertex), (const GLvoid*)12);
    glVertexAttribPointer(esl::ColorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(esl::Vertex), (const GLvoid*)20);

    glGenBuffers(1, &mesh.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(esl::uint), &mesh.indices[0], GL_STATIC_DRAW);

    mesh.fileInformation = nullptr;

    return mesh;
}

esl::LineMesh esl::GenerateLineMesh(esl::Line Line)
{
    esl::LineMesh mesh = {};

    mesh.lines.push_back(Line);
    mesh.vertices =
    {
        esl::LineVertex(Line.pointA, Line.pointB, Line.pointA),
        esl::LineVertex(Line.pointA, Line.pointB, Line.pointA),
        esl::LineVertex(Line.pointB, Line.pointB, Line.pointA),
        esl::LineVertex(Line.pointB, Line.pointB, Line.pointA)
    };
    mesh.indices = { 0, 1, 2, 1, 3, 2 };

    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(esl::LineVertex), &mesh.vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(esl::PositionAttribute);
    glEnableVertexAttribArray(esl::NextPositionAttribute);
    glEnableVertexAttribArray(esl::PreviousPositionAttribute);
    glVertexAttribPointer(esl::PositionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(esl::LineVertex), 0);
    glVertexAttribPointer(esl::NextPositionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(esl::LineVertex), (const GLvoid*)12);
    glVertexAttribPointer(esl::PreviousPositionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(esl::LineVertex), (const GLvoid*)24);

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

void esl::LoadOBJ(std::string FilePath, std::vector<glm::vec3>& Vertices, std::vector<esl::uint>& Indices)
{
    std::vector<std::string> lines;
    esl::GetFileLineByLine(FilePath, lines);
    for (int line = 0; line < lines.size(); line++)
    {
        std::vector<std::string> values;
        esl::FindValuesInLine(lines[line], values, ' ');
        if (values.size() == 0) continue;
        if (values[0] == "v")
            Vertices.push_back({ std::stof(values[1]), std::stof(values[2]), std::stof(values[3]) });
        else if (values[0] == "f")
        {
            for (int index = 0; index < 3; index++)
            {
                std::vector<std::string> indexValues;
                esl::FindValuesInLine(values[1 + index], indexValues, '/');
                Indices.push_back(std::stoi(indexValues[0]) - 1);
            }
        }
    }
}

void esl::LoadOBJWithPositionDataOnly(std::string FilePath, std::vector<esl::Vertex>& Vertices, std::vector<esl::uint>& Indices)
{
    Vertices.clear();
    Indices.clear();

    std::vector<glm::vec3> vertices;
    esl::LoadOBJ(FilePath, vertices, Indices);
    for (int vertex = 0; vertex < vertices.size(); vertex++)
    {
        Vertices.push_back(esl::Vertex(vertices[vertex], glm::vec2{ 0 }));
    }
}

void esl::LoadOBJ(std::string FilePath, std::vector<esl::Vertex>& Vertices, std::vector<esl::uint>& Indices)
{
    Vertices.clear();
    Indices.clear();

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

esl::Mesh esl::LoadOBJAsMesh(std::string FilePath, esl::VertexDescription VertexDescription)
{
    esl::Mesh mesh = {};

    mesh.fileInformation = std::make_shared<esl::FileInformation>();
    mesh.fileInformation->path = FilePath;
    mesh.fileInformation->lastWriteTime = std::filesystem::last_write_time(FilePath);

    if (VertexDescription == esl::VertexDescription::HasPosition)
        esl::LoadOBJWithPositionDataOnly(FilePath, mesh.vertices, mesh.indices);
    else
        esl::LoadOBJ(FilePath, mesh.vertices, mesh.indices);

    mesh.vertexDescription = VertexDescription;

    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(esl::Vertex), &mesh.vertices[0], GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(esl::PositionAttribute);
    glEnableVertexAttribArray(esl::UVAttribute);
    glEnableVertexAttribArray(esl::ColorAttribute);
    glVertexAttribPointer(esl::PositionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(esl::Vertex), 0);
    glVertexAttribPointer(esl::UVAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(esl::Vertex), (const GLvoid*)12);
    glVertexAttribPointer(esl::ColorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(esl::Vertex), (const GLvoid*)20);

    glGenBuffers(1, &mesh.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(esl::uint), &mesh.indices[0], GL_DYNAMIC_DRAW);

    return mesh;
}

void esl::ReloadMeshIfItHasFile(esl::Mesh& Mesh)
{
    if (Mesh.fileInformation != nullptr && Mesh.fileInformation->path != "")
    {
        if (Mesh.vertexDescription == esl::VertexDescription::HasPosition)
            esl::LoadOBJWithPositionDataOnly(Mesh.fileInformation->path, Mesh.vertices, Mesh.indices);
        else
            esl::LoadOBJ(Mesh.fileInformation->path, Mesh.vertices, Mesh.indices);

        Mesh.fileInformation->lastWriteTime = std::filesystem::last_write_time(Mesh.fileInformation->path);

        // remake mesh
        glBindVertexArray(Mesh.vao);

        glBindBuffer(GL_ARRAY_BUFFER, Mesh.vbo);
        glBufferData(GL_ARRAY_BUFFER, Mesh.vertices.size() * sizeof(esl::Vertex), &Mesh.vertices[0], GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Mesh.ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Mesh.indices.size() * sizeof(esl::uint), &Mesh.indices[0], GL_DYNAMIC_DRAW);
    }
}