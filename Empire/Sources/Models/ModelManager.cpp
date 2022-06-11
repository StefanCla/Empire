#include "Headers/Models/ModelManager.h"

ModelManager* ModelManager::Instance()
{
	static ModelManager* s_Instance = new ModelManager();
	return s_Instance;
}

ModelManager::ModelManager()
{
}

ModelManager::~ModelManager()
{
}

void ModelManager::AddModel(ModelType type, const ModelData& data)
{
	std::map<ModelType, std::shared_ptr<Model>>::iterator it = m_Models.find(type);

	if (it != m_Models.end())
		it->second = std::make_shared<Model>(data.Vertices, data.Indices);
	else
		m_Models.insert({ type, std::make_shared<Model>(data.Vertices, data.Indices) });
}

void ModelManager::AddModel(ModelType type, const std::string& filepath)
{
	std::map<ModelType, std::shared_ptr<Model>>::iterator it = m_Models.find(type);

	if (it != m_Models.end())
		it->second = std::make_shared<Model>(filepath);
	else
		m_Models.insert({ type, std::make_shared<Model>(filepath) });
}

std::shared_ptr<Model> ModelManager::GetModel(ModelType type) const
{
	std::map<ModelType, std::shared_ptr<Model>>::const_iterator it = m_Models.find(type);

	if (it != m_Models.end())
		return it->second;
	else
		return nullptr;
}

void ModelManager::AddShader(ShaderType type, const std::string& vertexPath, const std::string& fragmentPath)
{
	std::map<ShaderType, std::shared_ptr<Shader>>::iterator it = m_Shaders.find(type);

	if (it != m_Shaders.end())
		it->second = std::make_shared<Shader>(vertexPath, fragmentPath);
	else
		m_Shaders.insert({ type, std::make_shared<Shader>(vertexPath, fragmentPath) });
}

std::shared_ptr<Shader> ModelManager::GetShader(ShaderType type) const
{
	std::map<ShaderType, std::shared_ptr<Shader>>::const_iterator it = m_Shaders.find(type);

	if (it != m_Shaders.end())
		return it->second;
	else
		return nullptr;
}
