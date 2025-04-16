#include <SDL.h>
#include <SDL_joystick.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

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
#include "resource_functions.h"
#include "internal_mesh_functions.h"
#include "input.h"
#include "game.h"

namespace esl_main
{
	struct DisplayMode;
	esl_main::WindowType windowType;
	glm::vec2 windowSize;
	glm::vec2 drawableSize;
	std::vector<esl_main::DisplayMode> displayModes;
	int displayModeIndex;
	bool shouldApplyWindowSettings;
	bool isRunning;

	void CreateWindow(SDL_Window*& SDLWindow, SDL_GLContext& SDLGLContext);
	void DestroyWindow(SDL_Window* SDLWindow, SDL_GLContext SDLGLContext);
	void UpdateRenderTargetTexture(esl::Resources* Resources);
	void ApplyWindowSettings(SDL_Window* SDLWindow, esl::Camera& Camera);
	void HandleEvent(SDL_Event SDLEvent, SDL_Window* SDLWindow, esl::Input* const Input, esl::Resources* Resources);
}

int main(int Count, char* Values[])
{
	// initialize gl and sdl
	SDL_Window* sdlWindow;
	SDL_GLContext sdlGLContext;
	esl_main::CreateWindow(sdlWindow, sdlGLContext);

	std::unique_ptr<esl::Input> input = std::make_unique<esl::Input>();
	std::shared_ptr<esl::Resources> resources = std::make_shared<esl::Resources>();
	std::unique_ptr<esl::SpriteSystem> spriteSystem = std::make_unique<esl::SpriteSystem>();
	std::unique_ptr<esl::RenderSystem> renderSystem = std::make_unique<esl::RenderSystem>();

	SDL_Event sdlEvent;
	esl::uint lastTime = 0;
	esl_main::isRunning = true;

	glm::vec2 windowSize = esl_main::windowSize;
	resources->camera.SetProjectionSettings(windowSize.x / windowSize.y, 60.0f, 0.1f, 100.0f);
	resources->camera.SetViewSettings(glm::vec3(0, 0, 10), 0, 0);

	// init render target
	resources->renderTarget.mesh = esl::GenerateQuadMesh(glm::vec2(2, 2), glm::vec2(1, 1));
	resources->renderTarget.shader = esl::AddShader(resources, "shaders/renderTargetVertexShader.txt", "shaders/renderTargetFragmentShader.txt");
	esl_main::UpdateRenderTargetTexture(resources.get());

	// set material to draw lines
	resources->internalLineMaterial = esl::AddMaterial(resources, 0, esl::AddShader(resources, "shaders/lineVertexShader.txt", "shaders/lineFragmentShader.txt"));

	esl_main::OnGameStart(resources);

	while (esl_main::isRunning)
	{
		esl::uint currentTime = SDL_GetTicks();

		const float MaximumDeltaTime = 0.0625f;
		float totalDeltaTime = 0.001f * (currentTime - lastTime);
		while (totalDeltaTime > 0.0f)
		{
			float deltaTime = std::min(totalDeltaTime, MaximumDeltaTime);

			input->RecycleState();

			// apply window changes
			if (esl_main::shouldApplyWindowSettings)
			{
				esl_main::ApplyWindowSettings(sdlWindow, resources->camera);
				esl_main::UpdateRenderTargetTexture(resources.get());
				esl_main::shouldApplyWindowSettings = false;
			}
			// handle sdl events (input, window changes, etc)
			while (SDL_PollEvent(&sdlEvent) != 0)
			{
				esl_main::HandleEvent(sdlEvent, sdlWindow, input.get(), resources.get());
			}

			// set lock if needed, useful for mouselook
			SDL_SetRelativeMouseMode(input->mouse.isLockedToWindow ? SDL_TRUE : SDL_FALSE);

			// clear debug lines for this tick
			for (int lineMesh = 0; lineMesh < resources->internalLineMeshes.size(); lineMesh++)
			{
				resources->internalLineMeshes[lineMesh].lines.clear();
			}

			// tick event timers for delayed events
			for (int eventTimer = resources->internalEventTimers.size() - 1; eventTimer >= 0; eventTimer--)
			{
				resources->internalEventTimers[eventTimer].time -= deltaTime;
				if (resources->internalEventTimers[eventTimer].time <= 0.0f)
				{
					esl::InternalEventTimer timer = resources->internalEventTimers[eventTimer];
					esl::CallEvents(resources, timer.system, timer.id);
					resources->internalEventTimers.erase(resources->internalEventTimers.begin() + eventTimer);
				}
			}

			// queued systems when you need members set before calling start
			for (int system = resources->queuedSystems.size() - 1; system >= 0; system--)
			{
				resources->queuedSystems[system]->Start(resources);
				resources->systems.push_back(resources->queuedSystems[system]);
			}
			resources->queuedSystems.clear();

			for (int system = 0; system < resources->systems.size(); system++)
			{
				resources->systems[system]->Update(deltaTime, input.get(), resources);
			}

			// update sprite animations
			spriteSystem->UpdateSprites(resources, deltaTime);
			
			totalDeltaTime -= deltaTime;
		}

		esl_internal::UpdateLines(resources);
		esl_internal::UpdateTexts(resources);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(12.0f / 255.0f, 12.0f / 255.0f, 12.0f / 255.0f, 1.0f);

		renderSystem->RenderObjects(resources);

		SDL_GL_SwapWindow(sdlWindow);

 		lastTime = currentTime;
	}

	for (int font = 0; font < resources->fonts.size(); font++)
	{
		TTF_CloseFont(resources->fonts[font]->font);
	}

	glDeleteRenderbuffers(1, &resources->renderTarget.depthBufferName);
	glDeleteFramebuffers(1, &resources->renderTarget.frameBufferName);
	glDeleteTextures(1, &resources->renderTarget.textureName);

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
	spriteSystem.reset();
	renderSystem.reset();

	esl_main::DestroyWindow(sdlWindow, sdlGLContext);

	return 0;
}

void esl_main::CreateWindow(SDL_Window*& SDLWindow, SDL_GLContext& SDLGLContext)
{
	SDL_SetHint(SDL_HINT_WINDOWS_DPI_SCALING, "1");
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

	// Get resolution & refresh rates available from the primary monitor
	int displayModeCount = SDL_GetNumDisplayModes(0);
	SDL_DisplayMode displayMode;
	for (int mode = 0; mode < displayModeCount; mode++)
	{
		// zero is success for some reason
		if (SDL_GetDisplayMode(0, mode, &displayMode) == 0)
		{
			esl_main::displayModes.push_back
			({
				displayMode.format, { displayMode.w, displayMode.h },
				displayMode.refresh_rate
			});
		}
	}

	// pick a display mode to launch in
	esl_main::displayModeIndex = 6;
	glm::vec2 displayModeSize = esl_main::displayModes[esl_main::displayModeIndex].size;
	// set opengl version before creating window
	SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_CONTEXT_PROFILE_MASK, SDL_GLprofile::SDL_GL_CONTEXT_PROFILE_CORE);
	// create window, launch as hidden
	SDLWindow = SDL_CreateWindow("Beagle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, displayModeSize.x, displayModeSize.y, SDL_WindowFlags::SDL_WINDOW_OPENGL | SDL_WindowFlags::SDL_WINDOW_HIDDEN);
	// get true window size because of high dpi
	glm::ivec2 drawableWindowSize;
	SDL_GL_GetDrawableSize(SDLWindow, &drawableWindowSize.x, &drawableWindowSize.y);
	// correct window size from high dpi scaling
	SDL_SetWindowSize(SDLWindow, displayModeSize.x * (displayModeSize.x / drawableWindowSize.x), displayModeSize.y * (displayModeSize.y / drawableWindowSize.y));
	// now center and show window
	SDL_SetWindowPosition(SDLWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_ShowWindow(SDLWindow);

	// create gl context
	SDLGLContext = SDL_GL_CreateContext(SDLWindow);
	// set vsync
	SDL_GL_SetSwapInterval(1);

	// initialize image, ttf and glew
	if (!(IMG_Init(IMG_InitFlags::IMG_INIT_PNG) & (int)IMG_InitFlags::IMG_INIT_PNG))
		std::cout << "Error initializing SDL_image: " << IMG_GetError() << std::endl;
	if (TTF_Init() == -1)
		std::cout << "Error initializing SDL_ttf: " << TTF_GetError() << std::endl;
	GLenum glewInitState = glewInit();
	if (glewInitState != GLEW_OK)
		std::cout << "Error initializing GLEW: " << glewGetErrorString(glewInitState) << std::endl;
}

void esl_main::DestroyWindow(SDL_Window* SDLWindow, SDL_GLContext SDLGLContext)
{
	SDL_GL_DeleteContext(SDLGLContext);
	SDL_DestroyWindow(SDLWindow);

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void esl_main::UpdateRenderTargetTexture(esl::Resources* Resources)
{
	// delete render target
	glDeleteRenderbuffers(1, &Resources->renderTarget.depthBufferName);
	glDeleteFramebuffers(1, &Resources->renderTarget.frameBufferName);
	glDeleteTextures(1, &Resources->renderTarget.textureName);
	// remake it with new res
	glGenFramebuffers(1, &Resources->renderTarget.frameBufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, Resources->renderTarget.frameBufferName);
	glGenTextures(1, &Resources->renderTarget.textureName);
	glBindTexture(GL_TEXTURE_2D, Resources->renderTarget.textureName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, esl_main::drawableSize.x, esl_main::drawableSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glGenRenderbuffers(1, &Resources->renderTarget.depthBufferName);
	glBindRenderbuffer(GL_RENDERBUFFER, Resources->renderTarget.depthBufferName);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, esl_main::drawableSize.x, esl_main::drawableSize.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, Resources->renderTarget.depthBufferName);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, Resources->renderTarget.textureName, 0);
	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);
}

void esl_main::ApplyWindowSettings(SDL_Window* SDLWindow, esl::Camera& Camera)
{
	glm::vec2 displayModeSize = esl_main::displayModes[esl_main::displayModeIndex].size;
	Camera.aspectRatio = esl_main::windowSize.x / esl_main::windowSize.y;

	if (esl_main::windowType == esl_main::WindowType::Fullscreen)
	{
		// need to set to windowed before changing display mode
		if (SDL_SetWindowFullscreen(SDLWindow, 0) < 0)
			std::cout << SDL_GetError();

		// gets the desired mode and change the monitor resolution and refresh rate
		SDL_DisplayMode displayMode;
		SDL_GetDisplayMode(0, esl_main::displayModeIndex, &displayMode);
		if (SDL_SetWindowDisplayMode(SDLWindow, &displayMode) < 0)
		{
			std::cout << SDL_GetError();
		}

		// set to fullscreen
		if (SDL_SetWindowFullscreen(SDLWindow, SDL_WINDOW_FULLSCREEN) < 0)
			std::cout << SDL_GetError();

		esl_main::drawableSize = esl_main::windowSize = displayModeSize;
	}
	else
	{
		// need to set to windowed before changing window size
		if (SDL_SetWindowFullscreen(SDLWindow, 0) < 0)
			std::cout << SDL_GetError();

		// get best display mode available if it's borderless fullscreen
		if (esl_main::windowType == esl_main::WindowType::Borderless)
		{
			SDL_DisplayMode displayMode;
			SDL_GetDisplayMode(0, 0, &displayMode);
			displayModeSize = { displayMode.w, displayMode.h };
		}

		SDL_SetWindowSize(SDLWindow, displayModeSize.x, displayModeSize.y);
		glm::ivec2 drawableWindowSize;
		SDL_GL_GetDrawableSize(SDLWindow, &drawableWindowSize.x, &drawableWindowSize.y);
		// correct window size from "high dpi" scaling
		esl_main::windowSize = { displayModeSize.x * (displayModeSize.x / drawableWindowSize.x), displayModeSize.y * (displayModeSize.y / drawableWindowSize.y) };
		SDL_SetWindowSize(SDLWindow, esl_main::windowSize.x, esl_main::windowSize.y);

		// set to borderless or windowed
		if (SDL_SetWindowFullscreen(SDLWindow, (esl_main::windowType == esl_main::WindowType::Borderless) ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0) < 0)
			std::cout << SDL_GetError();
		SDL_SetWindowPosition(SDLWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

		esl_main::drawableSize = displayModeSize;
	}
}

void esl_main::HandleEvent(SDL_Event SDLEvent, SDL_Window* SDLWindow, esl::Input* const Input, esl::Resources* Resources)
{
	switch (SDLEvent.type)
	{
	case SDL_EventType::SDL_QUIT:
		esl_main::isRunning = false;
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
		Input->mouse.relativeMotion = glm::vec2(SDLEvent.motion.xrel, SDLEvent.motion.yrel);
		Input->mouse.position = glm::vec2(SDLEvent.motion.x, SDLEvent.motion.y);
		break;
	case SDL_EventType::SDL_WINDOWEVENT:
		if (SDLEvent.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
		{
			for (int texture = 0; texture < Resources->textures.size(); texture++)
			{
				if (Resources->textures[texture].fileInformation == nullptr) continue;
				auto lastWriteTime = Resources->textures[texture].fileInformation->lastWriteTime;
				auto newWriteTime = std::filesystem::last_write_time(Resources->textures[texture].fileInformation->path);
				if (lastWriteTime != newWriteTime)
				{
					esl::ReloadTexture(Resources->textures[texture]);
				}
			}

			for (int mesh = 0; mesh < Resources->meshes.size(); mesh++)
			{
				if (Resources->meshes[mesh].fileInformation != nullptr && Resources->meshes[mesh].fileInformation->path != "")
				{
					auto lastWriteTime = Resources->meshes[mesh].fileInformation->lastWriteTime;
					auto newWriteTime = std::filesystem::last_write_time(Resources->meshes[mesh].fileInformation->path);
					if (lastWriteTime != newWriteTime)
					{
						esl::ReloadMeshIfItHasFile(Resources->meshes[mesh]);
					}
				}
			}
		}
		break;
	}
}