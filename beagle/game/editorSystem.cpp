#include "editorSystem.h"
#include "globals.h"
#include "../texture.h"
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

	SpriteMesh* spriteMesh = Game->AddNewSpriteMesh({ 0.25, 0.25 }, { 0.125, 0.125 }, { 0.25f, 0.5f });
	spriteMesh->sprite.SetClip(AnimationClip{ 0, 1, 1 });

	Transform indicatorTransform = Transform();
	indicatorTransform.SetPosition(Vector2D{ -0.5, 0.5 });
	Game->objects.push_back(new Object((short)(Game->meshes.size() - 1), material, indicatorTransform, spriteMesh));
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
