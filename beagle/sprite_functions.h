#pragma once

#include <memory>

#include "resource_types.h"

namespace esl
{
	void PlayAnimationClip(std::shared_ptr<esl::Sprite> Sprite, esl::AnimationClip Clip);
	void QueueAnimationClip(std::shared_ptr<esl::Sprite> Sprite, esl::AnimationClip Clip);
}