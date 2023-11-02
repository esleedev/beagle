#include "editorSystem.h"
#include "globals.h"
#include "../texture.h"
#include <iostream>

void game_systems::EditorSystem::InitializeObjects(Game* Game)
{
	int shaderIndex;
	Game->AddNewShader("shaders/screenVertexShader.txt", "shaders/screenFragmentShader.txt", shaderIndex);

	GLuint editorIconsTexture = LoadTexture("textures/editorIcons.png");
	short material;
	Game->AddMaterial(editorIconsTexture, shaderIndex, material);

	Transform indicatorTransform = Transform();
	indicatorTransform.SetPosition(Vector2D{ -0.5, 0.5 });

	SpriteMesh* spriteMesh = Game->AddNewSpriteMesh({ 0.25, 0.25 }, { 0.125, 0.125 }, { 0.25f, 0.5f });
	spriteMesh->sprite.SetClip(AnimationClip{ 0, 1, 1 });

	Game->objects.push_back(new Object{ (short)(Game->meshes.size() - 1), material, indicatorTransform, spriteMesh });
}

void game_systems::EditorSystem::Update(float DeltaTime, Game* Game)
{
	if
	(
		Game->input->IsKeyJustPressed(SDL_SCANCODE_E) &&
		Game->input->isKeyPressed[SDL_SCANCODE_LCTRL]
	)
	{
		game_globals::appMode = game_globals::WorldEditor;
		InitializeObjects(Game);
		std::cout << "App mode: world editor" << std::endl;
	}
}
