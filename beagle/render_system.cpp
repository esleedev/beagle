#include <gl/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <algorithm>

#include "render_system.h"
#include "camera.h"

void esl::RenderSystem::RenderObjects(std::shared_ptr<esl::Resources> Resources)
{
    esl::Camera camera = Resources->camera;
    float verticalFieldOfViewInRadians = camera.verticalFieldOfViewInDegrees * 0.0174532925199f;
    glm::mat4x4 projectionMatrix = glm::perspective(verticalFieldOfViewInRadians, camera.aspectRatio, camera.near, camera.far);
    glm::mat4x4 viewMatrix = glm::lookAt(camera.position, camera.lookAt, glm::vec3(0, 1, 0));

    // sort objects by order of materials
    std::sort
    (
        Resources->objects.begin(), Resources->objects.end(),
        [](std::shared_ptr<esl::Object>& const A, std::shared_ptr<esl::Object>& const B) { return A->material < B->material; }
    );

    int object = 0;
    int objectCount = Resources->objects.size();
    for (short material = 0; material < Resources->materials.size(); material++)
    {
        short shader = Resources->materials[material].shader;
        glUseProgram(Resources->shaders[shader].program);
        glUniformMatrix4fv(Resources->shaders[shader].projectionMatrixUniform, 1, GL_FALSE, &projectionMatrix[0][0]);
        glUniformMatrix4fv(Resources->shaders[shader].viewMatrixUniform, 1, GL_FALSE, &viewMatrix[0][0]);

        // keep rendering objects that are using this material
        while (object < objectCount && Resources->objects[object]->material == material)
        {
            short mesh = Resources->objects[object]->mesh;
            glBindTexture(GL_TEXTURE_2D, Resources->textures[Resources->materials[material].texture].name);
            glBindVertexArray(Resources->meshes[mesh].vao);
            glUniformMatrix4fv(Resources->shaders[shader].objectMatrixUniform, 1, GL_FALSE, &Resources->objects[object]->transform.matrix[0][0]);
            glUniform1i(Resources->materials[material].texture, 0);
            glDrawElements(GL_TRIANGLES, Resources->meshes[mesh].indexCount, GL_UNSIGNED_INT, 0);
            object++;
        }
        // otherwise, move on to the next material...
    }
}