#include "input.h"

const char esl::Keyboard::LowerCaseLetters[26] =
{
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
	'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
	's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
};

const char esl::Keyboard::Numbers[10] =
{
	'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'
};

const char esl::Keyboard::MiscellaneousCharacters[13] =
{
	' ', '-', '=', '[', ']', '\\', ' ', ';', '\'', '`', ',', '.', '/'
};

char esl::Input::GetCharacterFromKey(SDL_Scancode ScanCode)
{
	if (ScanCode >= 4 && ScanCode <= 29)
		return esl::Keyboard::LowerCaseLetters[ScanCode - 4];
	else if (ScanCode >= 30 && ScanCode <= 39)
		return esl::Keyboard::Numbers[ScanCode - 30];
	else if (ScanCode >= 44 && ScanCode <= 56)
		return esl::Keyboard::MiscellaneousCharacters[ScanCode - 44];
	return 0;
}

bool esl::Input::IsKeyJustPressed(SDL_Scancode ScanCode)
{
	return keyboard.isKeyPressed[ScanCode] && !keyboard.wasKeyPressed[ScanCode];
}

bool esl::Input::IsKeyJustReleased(SDL_Scancode ScanCode)
{
	return !keyboard.isKeyPressed[ScanCode] && keyboard.wasKeyPressed[ScanCode];
}

bool esl::Input::IsMouseButtonJustPressed(esl::MouseButtonType MouseButtonType)
{
	esl::ubyte mouseButtonType = static_cast<esl::ubyte>(MouseButtonType);
	return mouse.isButtonPressed[mouseButtonType] && !mouse.wasButtonPressed[mouseButtonType];
}

bool esl::Input::IsMouseButtonJustReleased(esl::MouseButtonType MouseButtonType)
{
	esl::ubyte mouseButtonType = static_cast<esl::ubyte>(MouseButtonType);
	return !mouse.isButtonPressed[mouseButtonType] && mouse.wasButtonPressed[mouseButtonType];
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

	for (esl::ubyte button = 0; button < esl::Mouse::MaximumButtonCount; button++)
	{
		mouse.wasButtonPressed[button] = mouse.isButtonPressed[button];
	}

	for (esl::ubyte device = 0; device < esl::Gamepad::MaximumCount; device++)
	{
		for (esl::ubyte button = 0; button < esl::Gamepad::MaximumButtonCount; button++)
		{
			gamepads[device].wasButtonPressed[button] = gamepads[device].isButtonPressed[button];
		}
	}
}