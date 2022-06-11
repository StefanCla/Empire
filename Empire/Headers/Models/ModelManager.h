#pragma once
#include "Headers/Precomp.h"
#include "Headers/Models/Geometry.h"
#include "Headers/Models/Model.h"
#include "Headers/Renderer/Shader.h"
#include <map>
#include <memory>

class Model;

class ModelManager
{
public:
	ModelManager(const ModelManager& mm) = delete;
	ModelManager& operator=(const ModelManager& mm) = delete;

	static ModelManager* Instance();

	void AddModel(ModelType type, const ModelData& data);
	void AddModel(ModelType type, const std::string& filepath);
	std::shared_ptr<Model> GetModel(ModelType type) const;

	void AddShader(ShaderType type, const std::string& vertexPath, const std::string& fragmentPath);
	std::shared_ptr<Shader> GetShader(ShaderType type) const;

private:
	ModelManager();
	~ModelManager();

	std::map<ModelType, std::shared_ptr<Model>> m_Models;
	std::map<ShaderType, std::shared_ptr<Shader>> m_Shaders;
};
