#include <gl/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtx/quaternion.hpp>

#include <algorithm>

#include "render_system.h"
#include "camera.h"

void esl::RenderSystem::RenderObjects(std::shared_ptr<esl::Resources> Resources)
{
    esl::Camera camera = Resources->camera;

    float verticalFieldOfViewInRadians = camera.verticalFieldOfViewInDegrees * 0.0174532925199f;
    camera.projectionMatrix = glm::perspective(verticalFieldOfViewInRadians, camera.aspectRatio, camera.near, camera.far);

    // calculate camera look direction from euler
    glm::quat lookAtRotationAroundPitch = glm::angleAxis(glm::radians(camera.pitch), glm::vec3(1, 0, 0));
    glm::quat lookAtRotationAroundYaw = glm::angleAxis(glm::radians(camera.yaw), glm::vec3(0, 1, 0));
    glm::quat lookAtRotation = lookAtRotationAroundYaw * lookAtRotationAroundPitch;
    glm::vec3 lookAtDirection = lookAtRotation * glm::vec3(0, 0, -1);
    lookAtDirection = glm::normalize(lookAtDirection);

    camera.viewMatrix = glm::lookAt(camera.position, camera.position + lookAtDirection, glm::vec3(0, 1, 0));

    Resources->camera = camera;

    // calculate object matricies
    for (int object = 0; object < Resources->objects.size(); object++)
    {
        Resources->objects[object]->transform.matrix = glm::translate(glm::mat4x4(1), Resources->objects[object]->transform.position);
        Resources->objects[object]->transform.CalculateRotation();
        Resources->objects[object]->transform.matrix *= glm::toMat4(Resources->objects[object]->transform.rotation);
        Resources->objects[object]->transform.matrix *= glm::scale(glm::mat4x4(1), Resources->objects[object]->transform.scale);
    }

    // sort materials by render order
    std::sort
    (
        Resources->materials.begin(), Resources->materials.end(),
        [](std::shared_ptr<esl::Material>& const A, std::shared_ptr<esl::Material>& const B)
        {
            if (A->renderOrder != B->renderOrder)
                return A->renderOrder < B->renderOrder;

            if (A->shader != B->shader)
                return A->shader < B->shader;

            return A->texture < B->texture;
        }
    );

    // sort objects by order of materials
    std::sort
    (
        Resources->objects.begin(), Resources->objects.end(),
        [](std::shared_ptr<esl::Object>& const A, std::shared_ptr<esl::Object>& const B)
        {
            if (A->material->renderOrder != B->material->renderOrder)
                return A->material->renderOrder < B->material->renderOrder;

            if (A->material->shader != B->material->shader)
                return A->material->shader < B->material->shader;

            return A->material->texture < B->material->texture;
        }
    );

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int object = 0;
    int objectCount = Resources->objects.size();
    for (short material = 0; material < Resources->materials.size(); material++)
    {
        if (Resources->materials[material]->renderOrder >= esl::TransparentRenderOrder && (material == 0 || Resources->materials[material - 1]->renderOrder < esl::TransparentRenderOrder))
        {
            glDisable(GL_DEPTH_TEST);
        }

        short shader = Resources->materials[material]->shader;
        glUseProgram(Resources->shaders[shader].program);
        glUniformMatrix4fv(Resources->shaders[shader].projectionMatrixUniform, 1, GL_FALSE, &camera.projectionMatrix[0][0]);
        glUniformMatrix4fv(Resources->shaders[shader].viewMatrixUniform, 1, GL_FALSE, &camera.viewMatrix[0][0]);

        // keep rendering objects that are using this material
        while (object < objectCount && Resources->objects[object]->material == Resources->materials[material])
        {
            short mesh = Resources->objects[object]->mesh;
            glBindTexture(GL_TEXTURE_2D, Resources->textures[Resources->materials[material]->texture].name);
            glBindVertexArray(Resources->meshes[mesh].vao);
            glUniformMatrix4fv(Resources->shaders[shader].objectMatrixUniform, 1, GL_FALSE, &Resources->objects[object]->transform.matrix[0][0]);
            glUniform4fv(Resources->shaders[shader].colorUniform, 1, &Resources->objects[object]->color[0]);
            glDrawElements(GL_TRIANGLES, Resources->meshes[mesh].indices.size(), GL_UNSIGNED_INT, 0);
            object++;
        }
        // otherwise, move on to the next material...
    }
}