#include "editorSystem.h"
#include "globals.h"
#include "../texture.h"
#include <memory>
#include <iostream>

void game_systems::EditorSystem::InitializeAssetsAndObjects(Game* Game)
{
	short material;
	Game->AddMaterial
	(
		LoadTexture("textures/editorIcons.png"),
		Game->AddNewShader("shaders/screenVertexShader.txt", "shaders/screenFragmentShader.txt"),
		material
	);

	std::shared_ptr<SpriteMesh> spriteMesh = Game->AddSpriteMesh({ 0.25, 0.25 }, { 0.125, 0.125 }, { 0.25f, 0.5f });
	spriteMesh->sprite.SetClip(AnimationClip{ 0, 1, 1 });

	Transform cursorTransform = Transform();
	cursorTransform.SetPositionAndScale({ -0.5, 0.5, 0 }, { 3.0f / 4.0f, 1 });
	cursorObject = Game->AddObject(spriteMesh->meshIndex, material, cursorTransform);
}

void game_systems::EditorSystem::Update(float DeltaTime, Game* Game)
{
	// Toggle editor
	if
	(
		Game->input->IsKeyJustPressed(SDL_SCANCODE_E) &&
		Game->input->isKeyPressed[SDL_SCANCODE_LCTRL]
	)
	{
		if (game_globals::appMode == game_globals::AppMode::Game)
		{
			if (!wasInitialized)
				InitializeAssetsAndObjects(Game);
			wasInitialized = true;

			game_globals::appMode = game_globals::AppMode::WorldEditor;
			std::cout << "App mode: world editor" << std::endl;
		}
		else if (game_globals::appMode == game_globals::AppMode::WorldEditor)
		{
			game_globals::appMode = game_globals::AppMode::Game;
			std::cout << "App mode: game" << std::endl;
		}

		cursorObject->isEnabled = game_globals::appMode == game_globals::AppMode::WorldEditor;
	}

	// exit early if it's not in editor mode
	if (game_globals::appMode != game_globals::AppMode::WorldEditor)
		return;

	if (tool == game_systems::EditorTools::Line)
	{
	}
}
