#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>

#include "transform.h"

void esl::Transform::CalculateRotation()
{
    glm::quat rotateAroundPitch = glm::angleAxis(glm::radians(pitch), glm::vec3(1, 0, 0));
    glm::quat rotateAroundYaw = glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0));
    rotation = rotateAroundYaw * rotateAroundPitch;
}
