#include <SDL.h>
#include <SDL_joystick.h>

#include <gl/glew.h>
#include <SDL_opengl.h>

#include <iostream>
#include <memory>
#include <algorithm>

#include "common_types.h"
#include "sprite_system.h"
#include "render_system.h"
#include "mesh_functions.h"
#include "shader_functions.h"
#include "input.h"
#include "game.h"

#include "resource_functions.h"

namespace esl_main
{
	void CreateWindow(SDL_Window*& SDLWindow, SDL_GLContext& SDLGLContext);
	void DestroyWindow(SDL_Window* SDLWindow, SDL_GLContext SDLGLContext);
	void HandleEvent(SDL_Event SDLEvent, std::unique_ptr<esl::Input>& const Input, bool& IsRunning);
}

int main(int Count, char* Values[])
{
	// initialize gl and sdl
	SDL_Window* sdlWindow;
	SDL_GLContext sdlGLContext;
	esl_main::CreateWindow(sdlWindow, sdlGLContext);

	SDL_GL_SetSwapInterval(1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::unique_ptr<esl::Input> input = std::make_unique<esl::Input>();
	std::shared_ptr<esl::Resources> resources = std::make_shared<esl::Resources>();
	std::unique_ptr<esl::SpriteSystem> spriteSystem = std::make_unique<esl::SpriteSystem>();
	std::unique_ptr<esl::RenderSystem> renderSystem = std::make_unique<esl::RenderSystem>();

	SDL_Event sdlEvent;
	esl::uint lastTime = 0;
	bool isRunning = true;

	resources->camera.SetProjectionSettings(16.0f / 9.0f, 60.0f, 0.1f, 100.0f);
	resources->camera.SetViewSettings(glm::vec3(0, 0, 10), 0, 0);

	esl_main::OnGameStart(resources);

	while (isRunning)
	{
		input->RecycleState();

		while (SDL_PollEvent(&sdlEvent) != 0)
		{
			esl_main::HandleEvent(sdlEvent, input, isRunning);
		}

		esl::uint currentTime = SDL_GetTicks();

		const float MaximumDeltaTime = 0.05f;
		float totalDeltaTime = 0.001f * (currentTime - lastTime);
		while (totalDeltaTime > 0.0f)
		{
			float deltaTime = std::min(totalDeltaTime, MaximumDeltaTime);

			for (int system = 0; system < resources->systems.size(); system++)
			{
				resources->systems[system]->Update(deltaTime, input, resources);
			}

			spriteSystem->UpdateSprites(resources, deltaTime);
			
			totalDeltaTime -= deltaTime;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.75f, 0.75f, 0.75f, 1.0f);

		renderSystem->RenderObjects(resources);

		SDL_GL_SwapWindow(sdlWindow);

		lastTime = currentTime;
	}

	for (int texture = 0; texture < resources->textures.size(); texture++)
	{
		glDeleteTextures(1, &resources->textures[texture].name);
	}
	for (int shader = 0; shader < resources->shaders.size(); shader++)
	{
		esl::DeleteShaderProgram(resources->shaders[shader].program);
	}
	esl::DeleteMeshes(resources->meshes);

	input.reset();
	resources.reset();
	renderSystem.reset();
	spriteSystem.reset();

	esl_main::DestroyWindow(sdlWindow, sdlGLContext);

	return 0;
}

void esl_main::CreateWindow(SDL_Window*& SDLWindow, SDL_GLContext& SDLGLContext)
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

	SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_CONTEXT_PROFILE_MASK, SDL_GLprofile::SDL_GL_CONTEXT_PROFILE_CORE);

	SDLWindow = SDL_CreateWindow("Beagle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WindowFlags::SDL_WINDOW_OPENGL | SDL_WindowFlags::SDL_WINDOW_SHOWN);
	SDLGLContext = SDL_GL_CreateContext(SDLWindow);

	GLenum glewInitState = glewInit();
	if (glewInitState != GLEW_OK)
		std::cout << "Error initializing GLEW: " << glewGetErrorString(glewInitState) << std::endl;
}

void esl_main::DestroyWindow(SDL_Window* SDLWindow, SDL_GLContext SDLGLContext)
{
	SDL_GL_DeleteContext(SDLGLContext);
	SDL_DestroyWindow(SDLWindow);
	SDL_Quit();
}

void esl_main::HandleEvent(SDL_Event SDLEvent, std::unique_ptr<esl::Input>& const Input, bool& IsRunning)
{
	switch (SDLEvent.type)
	{
	case SDL_EventType::SDL_QUIT:
		IsRunning = false;
		break;
	case SDL_EventType::SDL_KEYDOWN:
		Input->keyboard.isKeyPressed[SDLEvent.key.keysym.scancode] = true;
		break;
	case SDL_EventType::SDL_KEYUP:
		Input->keyboard.isKeyPressed[SDLEvent.key.keysym.scancode] = false;
		break;
	case SDL_EventType::SDL_MOUSEBUTTONDOWN:
		if (SDLEvent.button.button >= 1 && SDLEvent.button.button <= 3)
		{
			Input->mouse.isButtonPressed[SDLEvent.button.button - 1] = true;
		}
		break;
	case SDL_EventType::SDL_MOUSEBUTTONUP:
		if (SDLEvent.button.button >= 1 && SDLEvent.button.button <= 3)
		{
			Input->mouse.isButtonPressed[SDLEvent.button.button - 1] = false;
		}
		break;
	case SDL_EventType::SDL_MOUSEMOTION:
		Input->mouse.position = glm::vec2(SDLEvent.motion.x, SDLEvent.motion.y);
		break;
	}
}