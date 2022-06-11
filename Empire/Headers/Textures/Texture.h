#pragma once
#include "Headers/Precomp.h"

enum class TextureType
{
	Diffuse, Emission
};

class Texture
{
public:
	Texture(TextureType type, const std::string& filepath);
	Texture(TextureType type, const std::vector<std::string>& filepath);
	~Texture();

	void Bind() const;
	void Unbind() const;

protected:
	unsigned int m_RendererID;
	unsigned int m_Slot;

	TextureType m_Type;
};
