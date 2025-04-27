#include "sprite_functions.h"

void esl::PlayAnimationClip(std::shared_ptr<esl::Sprite> Sprite, esl::AnimationClip Clip)
{
	Sprite->time = 0;
	Sprite->clip = Clip;
	Sprite->queuedClip = esl::AnimationClip(-1, 0, 0);
}

void esl::QueueAnimationClip(std::shared_ptr<esl::Sprite> Sprite, esl::AnimationClip Clip)
{
	Sprite->queuedClip = Clip;
}
