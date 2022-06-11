#include "Headers/Textures/Texture.h"
#include "Headers/FileHandler.h"

Texture::Texture(TextureType type, const std::string& filepath)
	: m_Slot(GL_TEXTURE_2D), m_Type(type)
{
	glGenTextures(1, &m_RendererID);
	glBindTexture(m_Slot, m_RendererID);

	int width, height, channels;
	char* image = FileHandler::Instance()->LoadImage(filepath.c_str(), &width, &height, &channels);

	if (channels == 3)
		glTexImage2D(m_Slot, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	else
		glTexImage2D(m_Slot, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glTexParameteri(m_Slot, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(m_Slot, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(m_Slot, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(m_Slot, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (glGetError() != GL_NO_ERROR)
		printf("Failed to initialise texture\n");
}

Texture::Texture(TextureType type, const std::vector<std::string>& filepaths)
	: m_Slot(GL_TEXTURE_CUBE_MAP), m_Type(type)
{
	glGenTextures(1, &m_RendererID);
	glBindTexture(m_Slot, m_RendererID);

	int width, height, channels;
	for (unsigned int i = 0; i < filepaths.size(); i++)
	{
		char* image = FileHandler::Instance()->LoadImage(filepaths[i].c_str(), &width, &height, &channels);
		if (channels == 3)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		else
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	}

	glTexParameteri(m_Slot, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(m_Slot, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(m_Slot, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glTexParameteri(m_Slot, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(m_Slot, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (glGetError() != GL_NO_ERROR)
		printf("Failed to initialise cubemap\n");
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind() const
{
	glActiveTexture(GL_TEXTURE0 + (GLenum)m_Type);
	glBindTexture(m_Slot, m_RendererID);
}

void Texture::Unbind() const
{
	glBindTexture(m_Slot, 0);
}
