#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace esl
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec2 uv;

        Vertex(glm::vec3 Position, glm::vec2 UV) : position(Position), uv(UV) {}
    };
}