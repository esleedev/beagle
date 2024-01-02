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
		bool isKeyPressed[SDL_Scancode::SDL_NUM_SCANCODES];
		bool wasKeyPressed[SDL_Scancode::SDL_NUM_SCANCODES];

		static const char LowerCaseLetters[26];
		static const char Numbers[10];
		static const char MiscellaneousCharacters[13];
	};

	struct Mouse
	{
		static const esl::ubyte MaximumButtonCount = 3;

		bool isButtonPressed[MaximumButtonCount];
		bool wasButtonPressed[MaximumButtonCount];
		glm::vec2 position;
	};

	enum class MouseButtonType : esl::ubyte
	{
		Left = 0,
		Middle = 1,
		Right = 2
	};

	class Input
	{
	public:
		esl::Keyboard keyboard;

		bool IsKeyJustPressed(SDL_Scancode ScanCode);
		bool IsKeyJustReleased(SDL_Scancode ScanCode);

		char GetCharacterFromKey(SDL_Scancode ScanCode);

		esl::Mouse mouse;
		
		bool IsMouseButtonJustPressed(esl::MouseButtonType MouseButtonType);
		bool IsMouseButtonJustReleased(esl::MouseButtonType MouseButtonType);

		esl::Gamepad gamepads[esl::Gamepad::MaximumCount];

		bool IsButtonJustPressed(esl::ubyte GamepadDevice, esl::ubyte Button);
		bool IsButtonJustReleased(esl::ubyte GamepadDevice, esl::ubyte Button);

		void RecycleState();
	};
}