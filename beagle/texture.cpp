#include "texture.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>

GLuint LoadTexture(const char* FilePath)
{
	GLuint texture;

	SDL_Surface* image = IMG_Load(FilePath);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D
	(
		GL_TEXTURE_2D,
		0, // Mip level
		image->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB,
		image->w, image->h, 0,
		image->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB,
		GL_UNSIGNED_BYTE, image->pixels
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	SDL_FreeSurface(image);

	return texture;
}