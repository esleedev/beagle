#include "sprite_functions.h"

void esl::PlayAnimationClip(std::shared_ptr<esl::Sprite> Sprite, esl::AnimationClip Clip)
{
	Sprite->time = 0;
	Sprite->clip = Clip;
	Sprite->shouldUpdateMesh = true;
}

void esl::QueueAnimationClip(std::shared_ptr<esl::Sprite> Sprite, esl::AnimationClip Clip)
{
	Sprite->queuedClip = Clip;
}
