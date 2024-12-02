#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace esl
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec2 uv;
        glm::vec3 color;

        Vertex() : position(0), uv(0), color(1) {}
        Vertex(glm::vec3 Position, glm::vec2 UV) : position(Position), uv(UV), color(1) {}
        Vertex(glm::vec3 Position, glm::vec2 UV, glm::vec3 Color) : position(Position), uv(UV), color(Color) {}
    };

    struct LineVertex
    {
        glm::vec3 position;
        glm::vec3 nextPosition;
        glm::vec3 previousPosition;
        LineVertex(glm::vec3 Position, glm::vec3 NextPosition, glm::vec3 PreviousPosition) : position(Position), nextPosition(NextPosition), previousPosition(PreviousPosition) {}
    };
}