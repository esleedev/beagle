#pragma once

#include <SDL_scancode.h>
#include <glm/vec2.hpp>

#include "common_types.h"

namespace esl
{
	struct Gamepad
	{
		static const esl::ubyte MaximumCount = 4;
		static const esl::ubyte MaximumButtonCount = 16;

		glm::vec2 leftStick, rightStick;
		bool isButtonPressed[MaximumButtonCount];
		bool wasButtonPressed[MaximumButtonCount];
	};

	struct Keyboard
	{
		bool isKeyPressed[SDL_NUM_SCANCODES];
		bool wasKeyPressed[SDL_NUM_SCANCODES];
	};

	class Input
	{
	public:
		Keyboard keyboard;

		bool IsKeyJustPressed(SDL_Scancode ScanCode);
		bool IsKeyJustReleased(SDL_Scancode ScanCode);

		Gamepad gamepads[Gamepad::MaximumCount];

		bool IsButtonJustPressed(esl::ubyte GamepadDevice, esl::ubyte Button);
		bool IsButtonJustReleased(esl::ubyte GamepadDevice, esl::ubyte Button);

		void RecycleState();
	};
}