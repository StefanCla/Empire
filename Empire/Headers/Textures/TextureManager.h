#pragma once
#include "Headers/Precomp.h"
#include <map>
#include <memory>
#include "Headers/Textures/Texture.h"

class TextureManager
{
public:
	TextureManager(const TextureManager& tm) = delete;
	TextureManager& operator=(const TextureManager& tm) = delete;

	static TextureManager* Instance();

	void AddTexture(TextureType type, const std::string& name, const std::string& filepath);
	void AddCubeMap(const std::string& name, const std::vector<std::string>& filepaths);
	std::shared_ptr<Texture> GetTexture(const std::string& name) const;

private:
	TextureManager();
	~TextureManager();

	std::map<std::string, std::shared_ptr<Texture>> m_Textures;
};
