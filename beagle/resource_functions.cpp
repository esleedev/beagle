#include <gl/glew.h>
#include <SDL_image.h>

#include "resource_functions.h"
#include "shader_functions.h"

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
	short Texture, short Shader, short RenderOrder
)
{
	std::shared_ptr<esl::Material> material = std::make_shared<esl::Material>
	(
		Texture, Shader, RenderOrder
	);
	Resources->materials.push_back(material);
	return material;
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

std::shared_ptr<esl::Text> esl::AddText
(
	std::shared_ptr<esl::Resources> Resources, short SharedMesh,
	glm::vec2 Position, glm::vec3 Color, std::string Text,
	esl::HorizontalTextAlignment HorizontalAlignment, esl::VerticalTextAlignment VerticalAlignment,
	float LetterSpacing, float Size
)
{
	std::shared_ptr<esl::Text> text = std::make_shared<esl::Text>();
	text->sharedMesh = SharedMesh;
	text->position = Position;
	text->color = Color;
	text->text = Text;
	text->horizontalAlignment = HorizontalAlignment;
	text->verticalAlignment = VerticalAlignment;
	text->letterSpacing = LetterSpacing;
	text->size = Size;
	Resources->texts.push_back(text);
	return text;
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

void esl::CallEvents(std::shared_ptr<esl::Resources> Resources, esl::System* System, esl::ushort Id)
{
	for (int index = 0; index < Resources->events.size(); index++)
	{
		Resources->events[index]->OnEvent(System, Id);
	}
}