#pragma once

#include <SDL.h>
#include <string>
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

	bool isEnabled;

	Object(short Mesh, short Material, bool IsEnabled = true);
	Object(short Mesh, short Material, Transform Transform, bool IsEnabled = true);
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

	void AddMesh(Mesh Mesh, int& Index);
	template <typename SystemType> SystemType* AddNewSystem()
	{
		SystemType* system = new SystemType();
		systems.push_back(system);
		return system;
	}
	SpriteMesh* AddNewSpriteMesh(Vector2D Size, Vector2D Origin, Vector2D FrameUVSize);
	// index received can be different from shader program handle that opengl returns. it is mainly used for materials to know what shader in the list to use.
	void AddNewShader(std::string VertexShaderFilePath, std::string FragmentShaderFilePath, int& Index);
	int AddNewShader(std::string VertexShaderFilePath, std::string FragmentShaderFilePath);

	void AddMaterial(GLuint Texture, int Shader, short& Index);
};

extern void OnGameStart(Game* Game);
extern void OnGameEnd(Game* Game);