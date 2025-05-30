#include <gl/glew.h>
#include <iostream>
#include "sprite_system.h"

void esl::SpriteSystem::UpdateSprites(std::shared_ptr<esl::Resources> Resources, float DeltaTime)
{
	for (int sprite = 0; sprite < Resources->sprites.size(); sprite++)
	{
		std::shared_ptr<esl::Sprite> thisSprite = Resources->sprites[sprite];
        thisSprite->time += DeltaTime * thisSprite->clip.speed;
		if (thisSprite->time >= thisSprite->clip.frameCount || (thisSprite->clip.speed <= 0.0f && thisSprite->queuedClip.frameStart >= 0 && thisSprite->clip != thisSprite->queuedClip))
		{
			thisSprite->clip = thisSprite->queuedClip;
            thisSprite->queuedClip = esl::AnimationClip(-1, 0, 0);
			thisSprite->time = 0;
		}

        // update mesh uvs if frame changed
        int frame = glm::floor(thisSprite->time + thisSprite->clip.frameStart);
        if (frame != thisSprite->frameOnLastUpdate)
        {
            int xCount = glm::floor(1.0f / thisSprite->frameUVSize.x);
            int xFrame = frame % xCount;
            int yFrame = glm::floor(frame / xCount);

            float uvX = xFrame * thisSprite->frameUVSize.x;
            float uvY = yFrame * thisSprite->frameUVSize.y;
            float uvWidth = thisSprite->frameUVSize.x;
            float uvHeight = thisSprite->frameUVSize.y;

            Resources->meshes[thisSprite->mesh].vertices[0].uv = { uvX, uvY + uvHeight };
            Resources->meshes[thisSprite->mesh].vertices[1].uv = { uvX + uvWidth, uvY + uvHeight };
            Resources->meshes[thisSprite->mesh].vertices[2].uv = { uvX, uvY };
            Resources->meshes[thisSprite->mesh].vertices[3].uv = { uvX + uvWidth, uvY };

            glBindVertexArray(Resources->meshes[thisSprite->mesh].vao);
            glBindBuffer(GL_ARRAY_BUFFER, Resources->meshes[thisSprite->mesh].vbo);
            glBufferData(GL_ARRAY_BUFFER, Resources->meshes[thisSprite->mesh].vertices.size() * sizeof(esl::Vertex), &Resources->meshes[thisSprite->mesh].vertices[0], GL_STATIC_DRAW);
            glBindVertexArray(0);

            thisSprite->frameOnLastUpdate = frame;
        }
	}
}
