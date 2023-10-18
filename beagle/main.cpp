#include <stdio.h>
#include <SDL.h>
#include <SDL_joystick.h>
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <algorithm>
#include "game.h"

int GetGamepadDeviceIndex(SDL_Joystick* joysticks[], int InstanceID)
{
	for (Uint8 device = 0; device < Input::MaximumGamepadCount; device++)
	{
		if (joysticks[device] != nullptr && SDL_JoystickInstanceID(joysticks[device]) == InstanceID)
		{
			return device;
		}
	}
	return -1;
}

int main(int argc, char* args[])
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_Window* sdlWindow = SDL_CreateWindow("Beagle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	SDL_GLContext sdlGLContext = SDL_GL_CreateContext(sdlWindow);

	// glewExperimental = GL_TRUE;
	GLenum glewInitState = glewInit();
	if (glewInitState != GLEW_OK)
		printf("Error initializing GLEW: %s\n", glewGetErrorString(glewInitState));

	SDL_GL_SetSwapInterval(1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Game* game = new Game();
	Uint32 lastTime = 0;

	SDL_Event sdlEvent;
	bool isRunning = true;
	
	SDL_Joystick* gamepads[Input::MaximumGamepadCount];

	while (isRunning)
	{
		for (int key = 0; key < SDL_NUM_SCANCODES; key++)
		{
			game->input->wasKeyPressed[key] = game->input->isKeyPressed[key];
		}

		for (Uint8 device = 0; device < Input::MaximumGamepadCount; device++)
		{
			for (Uint8 button = 0; button < GamepadInput::MaximumButtonCount; button++)
			{
				game->input->gamepadInputs[device].wasButtonPressed[button] = game->input->gamepadInputs[device].isButtonPressed[button];
			}
			//game->input->gamepadInputs[device].leftStick.x = 0;
			//game->input->gamepadInputs[device].leftStick.y = 0;
			//game->input->gamepadInputs[device].rightStick.x = 0;
			//game->input->gamepadInputs[device].rightStick.y = 0;
		}

		while (SDL_PollEvent(&sdlEvent) != 0)
		{
			switch (sdlEvent.type)
			{
				case SDL_QUIT:
					isRunning = false;
					break;
				case SDL_KEYDOWN:
					game->input->isKeyPressed[sdlEvent.key.keysym.scancode] = true;
					break;
				case SDL_KEYUP:
					game->input->isKeyPressed[sdlEvent.key.keysym.scancode] = false;
					break;

				// gamepad events
				case SDL_JOYDEVICEADDED:
				{
					int deviceIndex = sdlEvent.jdevice.which;
					if (deviceIndex >= 0 && deviceIndex < Input::MaximumGamepadCount)
					{
						gamepads[deviceIndex] = SDL_JoystickOpen(deviceIndex);
					}
					break;
				}
				case SDL_JOYAXISMOTION:
				{
					int instanceID = sdlEvent.jdevice.which;
					int deviceIndex = GetGamepadDeviceIndex(gamepads, instanceID);
					if (deviceIndex >= 0 && gamepads[deviceIndex] != nullptr && SDL_JoystickInstanceID(gamepads[deviceIndex]) == instanceID)
					{
						float axisValue;
						if (sdlEvent.jaxis.value >= 0)
							axisValue = (float)sdlEvent.jaxis.value / 32767.0f;
						else
							axisValue = (float)sdlEvent.jaxis.value / 32768.0f;

						if (sdlEvent.jaxis.axis == 0)
							game->input->gamepadInputs[deviceIndex].leftStick.x = axisValue;
						else if (sdlEvent.jaxis.axis == 1)
							game->input->gamepadInputs[deviceIndex].leftStick.y = axisValue;
						else if (sdlEvent.jaxis.axis == 2)
							game->input->gamepadInputs[deviceIndex].rightStick.x = axisValue;
						else if (sdlEvent.jaxis.axis == 3)
							game->input->gamepadInputs[deviceIndex].rightStick.y = axisValue;
					}
					break;
				}
				case SDL_JOYBUTTONDOWN:
				{
					int instanceID = sdlEvent.jdevice.which;
					int deviceIndex = GetGamepadDeviceIndex(gamepads, instanceID);
					if (deviceIndex >= 0 && sdlEvent.jbutton.button < GamepadInput::MaximumButtonCount)
					{
						game->input->gamepadInputs[deviceIndex].isButtonPressed[sdlEvent.jbutton.button] = true;
					}
					break;
				}
				case SDL_JOYBUTTONUP:
				{
					int instanceID = sdlEvent.jdevice.which;
					int deviceIndex = GetGamepadDeviceIndex(gamepads, instanceID);
					if (deviceIndex >= 0 && sdlEvent.jbutton.button < GamepadInput::MaximumButtonCount)
					{
						game->input->gamepadInputs[deviceIndex].isButtonPressed[sdlEvent.jbutton.button] = false;
					}
					break;
				}
			}
		}

		Uint32 currentTime = SDL_GetTicks();

		game->Update(0.001 * (currentTime - lastTime));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.25f, 0.25f, 0.2f, 1.0f);

		game->Render();

		SDL_GL_SwapWindow(sdlWindow);

		lastTime = currentTime;
	}

	delete game;

	SDL_GL_DeleteContext(sdlGLContext);
	SDL_DestroyWindow(sdlWindow);
	SDL_Quit();

	return 0;
}