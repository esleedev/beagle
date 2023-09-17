#include <stdio.h>
#include <SDL.h>
#include <gl/glew.h>
#include <SDL_opengl.h>

#include "game.h"

int main(int argc, char* args[])
{
	SDL_Init(SDL_INIT_VIDEO);

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

	Game* game = new Game();
	Uint32 lastTime = 0;

	SDL_Event sdlEvent;
	bool isRunning = true;
	while (isRunning)
	{
		while (SDL_PollEvent(&sdlEvent) != 0)
		{
			if (sdlEvent.type == SDL_QUIT)
				isRunning = false;
		}

		Uint32 currentTime = SDL_GetTicks();

		game->Update(0.001 * (currentTime - lastTime));

		glClear(GL_COLOR_BUFFER_BIT);
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

