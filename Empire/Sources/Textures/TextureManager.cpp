#include "Headers/Textures/TextureManager.h"

TextureManager* TextureManager::Instance()
{
	static TextureManager* s_Instance = new TextureManager;
	return s_Instance;
}

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}

void TextureManager::AddTexture(TextureType type, const std::string& name, const std::string& filepath)
{
	std::map<std::string, std::shared_ptr<Texture>>::iterator it = m_Textures.find(name);

	if (it != m_Textures.end())
		it->second = std::make_shared<Texture>(type, filepath);
	else
		m_Textures.insert({ name, std::make_shared<Texture>(type, filepath) });
}

void TextureManager::AddCubeMap(const std::string& name, const std::vector<std::string>& filepaths)
{
	std::map<std::string, std::shared_ptr<Texture>>::iterator it = m_Textures.find(name);

	if (it != m_Textures.end())
		it->second = std::make_shared<Texture>(TextureType::Diffuse, filepaths);
	else
		m_Textures.insert({ name, std::make_shared<Texture>(TextureType::Diffuse, filepaths) });
}

std::shared_ptr<Texture> TextureManager::GetTexture(const std::string& name) const
{
	std::map<std::string, std::shared_ptr<Texture>>::const_iterator it = m_Textures.find(name);

	if (it != m_Textures.end())
		return it->second;
	else
		return nullptr;
}
