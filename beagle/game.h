#pragma once

#include <SDL.h>
#include "graphicsTypes.h"
#include "transform.h"

class Game;

class System
{
public:
	virtual void Update(float DeltaTime, Game* Game) = 0;
};

class Object
{
public:
	short mesh;
	short material;
	Transform transform;
	SpriteMesh* spriteMesh;

	Object(short Mesh, short Material);
	Object(short Mesh, short Material, Transform Transform, SpriteMesh* SpriteMesh);
};

struct GamepadInput
{
	Vector2D leftStick, rightStick;
	const static Uint8 MaximumButtonCount = 16;
	bool isButtonPressed[MaximumButtonCount];
	bool wasButtonPressed[MaximumButtonCount];
};

class Input
{
public:
	// keyboard
	bool isKeyPressed[SDL_NUM_SCANCODES];
	bool wasKeyPressed[SDL_NUM_SCANCODES];

	bool IsKeyJustPressed(SDL_Scancode ScanCode)
	{
		return isKeyPressed[ScanCode] && !wasKeyPressed[ScanCode];
	}

	bool IsKeyJustReleased(SDL_Scancode ScanCode)
	{
		return !isKeyPressed[ScanCode] && wasKeyPressed[ScanCode];
	}

	// gamepads
	const static Uint8 MaximumGamepadCount = 6;
	GamepadInput gamepadInputs[Input::MaximumGamepadCount];

	bool IsButtonJustPressed(Uint8 GamepadDevice, Uint8 Button)
	{
		return gamepadInputs[GamepadDevice].isButtonPressed[Button] && !gamepadInputs[GamepadDevice].wasButtonPressed[Button];
	}

	bool IsButtonJustReleased(Uint8 GamepadDevice, Uint8 Button)
	{
		return !gamepadInputs[GamepadDevice].isButtonPressed[Button] && gamepadInputs[GamepadDevice].wasButtonPressed[Button];
	}
};

class Game
{
public:
	std::vector<Mesh> meshes;
	std::vector<Material> materials;
	std::vector<Shader> shaders;
	std::vector<Object*> objects;
	std::vector<System*> systems;
	std::vector<SpriteMesh*> spriteMeshes;
	std::vector<DynamicMesh*> dynamicMeshes;

	Matrix projectionMatrix;
	Transform cameraTransform;

	// projection settings
	float aspectRatio, verticalFieldOfViewInDegrees, near, far;

	Input* input;

	Game();
	~Game();

	void Update(float DeltaTime);
	void Render();

	void PushBackMesh(Mesh Mesh, int& Index);
	template <typename SystemType> SystemType* PushBackSystem(SystemType* System)
	{
		systems.push_back(System);
		return System;
	}
};

extern void OnGameStart(Game* Game);
extern void OnGameEnd(Game* Game);