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

	Transform indicatorTransform = Transform();
	indicatorTransform.SetPositionAndScale({ -0.5, 0.5, 0 }, { 3.0f / 4.0f, 1 });
	Game->objects.push_back(new Object(spriteMesh->meshIndex, material, indicatorTransform));
	indicatorObjectIndex = Game->objects.size() - 1;
}

void game_systems::EditorSystem::Update(float DeltaTime, Game* Game)
{
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

		Game->objects[indicatorObjectIndex]->isEnabled = game_globals::appMode == game_globals::AppMode::WorldEditor;
	}
}
