#include <corecrt_math.h>
#include "graphicsTypes.h"

void Sprite::SetClip(AnimationClip Clip, bool ShouldResetTimeIfNewClip)
{
	clip = Clip;
	queuedClip = Clip;

	if
	(
		ShouldResetTimeIfNewClip &&
		(clip.frameStart != Clip.frameStart || clip.frameCount != Clip.frameCount)
	)
	{
		time = 0;
	}
}

void Sprite::QueueClip(AnimationClip Clip)
{
	queuedClip = Clip;
}

Rectangle Sprite::GetUVs()
{
	int frame = floor(time + clip.frameStart);
	int xCount = floor(1.0f / frameUVSize.x);
	int xFrame = frame % xCount;
	int yFrame = floor(frame / xCount);

	return
	{
		xFrame * frameUVSize.x,
		yFrame * frameUVSize.y,
		frameUVSize.x,
		frameUVSize.y
	};
}