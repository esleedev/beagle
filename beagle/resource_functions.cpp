#include <gl/glew.h>
#include <SDL_image.h>
#include <iostream>

#include "resource_functions.h"
#include "shader_functions.h"
#include "mesh_functions.h"

short esl::AddShader
(
	std::shared_ptr<esl::Resources> Resources,
	std::string VertexShaderFilePath, std::string FragmentShaderFilePath
)
{
	esl::uint program = esl::CreateShaderProgram(VertexShaderFilePath, FragmentShaderFilePath);
	esl::Shader shader = esl::GetShaderWithLocations(program);
	Resources->shaders.push_back(shader);
	return Resources->shaders.size() - 1;
}

short esl::AddTexture(std::shared_ptr<esl::Resources> Resources, const char* FilePath)
{
	esl::Texture texture;

	SDL_Surface* image = IMG_Load(FilePath);

	glGenTextures(1, &texture.name);
	glBindTexture(GL_TEXTURE_2D, texture.name);
	glTexImage2D
	(
		GL_TEXTURE_2D,
		0, // Mip level
		image->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB,
		image->w, image->h, 0,
		image->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB,
		GL_UNSIGNED_BYTE, image->pixels
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	SDL_FreeSurface(image);

	texture.fileInformation = std::make_shared<esl::FileInformation>();
	texture.fileInformation->path = FilePath;
	texture.fileInformation->lastWriteTime = std::filesystem::last_write_time(FilePath);

	Resources->textures.push_back(texture);
	return Resources->textures.size() - 1;
}

short esl::GetTexture(std::shared_ptr<esl::Resources> Resources, const char* FilePath)
{
	for (short index = 0; index < Resources->textures.size(); index++)
	{
		if (Resources->textures[index].fileInformation->path == FilePath)
			return index;
	}
	return -1;
}

void esl::ReloadTexture(esl::Texture& Texture)
{
	SDL_Surface* image = IMG_Load(Texture.fileInformation->path.c_str());

	glBindTexture(GL_TEXTURE_2D, Texture.name);
	glTexImage2D
	(
		GL_TEXTURE_2D,
		0, // Mip level
		image->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB,
		image->w, image->h, 0,
		image->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB,
		GL_UNSIGNED_BYTE, image->pixels
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	Texture.fileInformation->lastWriteTime = std::filesystem::last_write_time(Texture.fileInformation->path);

	SDL_FreeSurface(image);
}

std::shared_ptr<esl::Material> esl::AddMaterial
(
	std::shared_ptr<esl::Resources> Resources,
	short Texture, short Shader, short RenderOrder,
	std::string Name
)
{
	std::shared_ptr<esl::Material> material = std::make_shared<esl::Material>
	(
		Texture, Shader, RenderOrder
	);

	if (Name.size() > 0)
	{
		material->name = Name;
		esl::ushort hash = GetHashFromName(Name) % esl::HashIndexedMaterialsSize;
		while (true)
		{
			if (Resources->internalHashIndexedMaterials[hash] == nullptr)
			{
				Resources->internalHashIndexedMaterials[hash] = material;
				break;
			}
			hash = (hash + 1) % esl::HashIndexedMaterialsSize;
		}
	}

	Resources->materials.push_back(material);
	return material;
}

std::shared_ptr<esl::Material> esl::GetMaterial(std::shared_ptr<esl::Resources> Resources, std::string Name)
{
	if (Name.size() > 0)
	{
		esl::ushort hash = GetHashFromName(Name) % esl::HashIndexedMaterialsSize;
		while (true)
		{
			if (Resources->internalHashIndexedMaterials[hash] == nullptr) break;
			if (Resources->internalHashIndexedMaterials[hash]->name == Name)
			{
				return Resources->internalHashIndexedMaterials[hash];
			}
			hash = (hash + 1) % esl::HashIndexedMaterialsSize;
		}
	}

	return nullptr;
}

esl::ushort esl::GetHashFromName(std::string Name)
{
	int size = Name.size();
	int value = 0;
	int powersOfTen[] = { 1, 10, 100, 1000, 10000, 100000 };
	for (int character = 0; character < size; character++)
	{
		value += powersOfTen[character % 6] * (Name[character] % 128);
	}
	return value % 65535;
}

short esl::AddMesh(std::shared_ptr<esl::Resources> Resources, esl::Mesh Mesh)
{
	Resources->meshes.push_back(Mesh);
	return Resources->meshes.size() - 1;
}

std::shared_ptr<esl::Sprite> esl::AddSprite(std::shared_ptr<esl::Resources> Resources, short Mesh, glm::vec2 FrameUVSize, esl::AnimationClip InitialClip)
{
	std::shared_ptr<esl::Sprite> sprite = std::make_shared<esl::Sprite>
	(
		Mesh, FrameUVSize, InitialClip, esl::AnimationClip(-1, 0, 0)
	);

	Resources->sprites.push_back(sprite);
	return sprite;
}

short esl::AddFont(std::shared_ptr<esl::Resources> Resources, std::string FilePath, int Size)
{
	TTF_Font* font = TTF_OpenFont(FilePath.c_str(), Size);
	Resources->fonts.push_back(font);
	return Resources->fonts.size() - 1;
}

std::shared_ptr<esl::Object> esl::AddTextObject
(
	std::shared_ptr<esl::Resources> Resources,
	short Shader, short Font,
	std::string Text, glm::vec4 DiffuseColor,
	short RenderOrder,
	esl::HorizontalTextAlignment HorizontalTextAlignment,
	esl::VerticalTextAlignment VerticalTextAlignment
)
{
	// todo: cache font or use a shared texture
	esl::Texture texture;

	SDL_Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	SDL_Surface* image = TTF_RenderText_Blended(Resources->fonts[Font], Text.c_str(), color);
	glGenTextures(1, &texture.name);
	glBindTexture(GL_TEXTURE_2D, texture.name);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, image->pitch / image->format->BytesPerPixel);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D
	(
		GL_TEXTURE_2D,
		0, // Mip level
		GL_RGBA,
		image->w, image->h, 0,
		GL_BGRA,
		GL_UNSIGNED_BYTE, image->pixels
	);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	Resources->textures.push_back(texture);

	glm::vec2 origin = { 0, 0 };
	if (HorizontalTextAlignment == esl::HorizontalTextAlignment::Middle)
		origin.x = 0.5f;
	else if (HorizontalTextAlignment == esl::HorizontalTextAlignment::Right)
		origin.x = 1;
	if (VerticalTextAlignment == esl::VerticalTextAlignment::Middle)
		origin.y = 0.5f;
	else if (VerticalTextAlignment == esl::VerticalTextAlignment::Bottom)
		origin.y = 1;

	glm::vec2 size = glm::vec2((float)image->w / (float)image->h * (image->h / 32.0f), image->h / 32.0f);
	short mesh = esl::AddMesh(Resources, esl::GenerateQuadMesh(size, origin * size));
	std::shared_ptr<esl::Material> material = esl::AddMaterial(Resources, Resources->textures.size() - 1, Shader, RenderOrder);
	std::shared_ptr<esl::Object> object = esl::AddObject(Resources, mesh, material, esl::Transform(), DiffuseColor);

	SDL_FreeSurface(image);

	return object;
}

std::shared_ptr<esl::Object> esl::AddObject(std::shared_ptr<esl::Resources> Resources, short Mesh, std::shared_ptr<esl::Material> Material, esl::Transform Transform, glm::vec4 DiffuseColor)
{
	std::shared_ptr<esl::Object> object = std::make_shared<esl::Object>();
	object->mesh = Mesh;
	object->material = Material;
	object->transform = Transform;
	object->diffuseColor = DiffuseColor;
	Resources->objects.push_back(object);
	return object;
}

void esl::CallEvents(std::shared_ptr<esl::Resources> Resources, std::shared_ptr<esl::System> System, esl::ushort ID, float WaitTime)
{
	if (WaitTime > 0.0f)
	{
		esl::InternalEventTimer timer;
		timer.id = ID;
		timer.system = System;
		timer.time = WaitTime;
		Resources->internalEventTimers.push_back(timer);
	}
	else
	{
		for (int index = 0; index < Resources->events.size(); index++)
		{
			if (Resources->events[index]->id == ID)
				Resources->events[index]->OnEvent(System);
		}
	}
}