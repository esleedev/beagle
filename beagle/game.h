#pragma once

#include <memory>
#include "resource_types.h"

namespace esl_main
{
	extern bool isRunning;
	extern void OnGameStart(std::shared_ptr<esl::Resources> Resources);

	enum class WindowType : esl::ubyte
	{
		Window = 0,
		Fullscreen,
		Borderless
	};
	struct DisplayMode
	{
		Uint32 pixelFormat;
		glm::ivec2 size;
		int refreshRate;
	};
	extern glm::vec2 windowSize;
	extern int displayModeIndex;
	extern esl_main::WindowType windowType;
	extern std::vector<esl_main::DisplayMode> displayModes;
	extern bool shouldApplyWindowSettings;
}