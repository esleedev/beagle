#include "input.h"

bool esl::Input::IsKeyJustPressed(SDL_Scancode ScanCode)
{
	return keyboard.isKeyPressed[ScanCode] && !keyboard.wasKeyPressed[ScanCode];
}

bool esl::Input::IsKeyJustReleased(SDL_Scancode ScanCode)
{
	return !keyboard.isKeyPressed[ScanCode] && keyboard.wasKeyPressed[ScanCode];
}

bool esl::Input::IsButtonJustPressed(esl::ubyte GamepadDevice, esl::ubyte Button)
{
	return gamepads[GamepadDevice].isButtonPressed[Button] && !gamepads[GamepadDevice].wasButtonPressed[Button];
}

bool esl::Input::IsButtonJustReleased(esl::ubyte GamepadDevice, esl::ubyte Button)
{
	return !gamepads[GamepadDevice].isButtonPressed[Button] && gamepads[GamepadDevice].wasButtonPressed[Button];
}

void esl::Input::RecycleState()
{
	for (int key = 0; key < SDL_Scancode::SDL_NUM_SCANCODES; key++)
	{
		keyboard.wasKeyPressed[key] = keyboard.isKeyPressed[key];
	}

	for (esl::ubyte device = 0; device < esl::Gamepad::MaximumCount; device++)
	{
		for (esl::ubyte button = 0; button < esl::Gamepad::MaximumButtonCount; button++)
		{
			gamepads[device].wasButtonPressed[button] = gamepads[device].isButtonPressed[button];
		}
	}
}