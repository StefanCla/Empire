#pragma once
#include "Headers/Precomp.h"
#include <memory>

class VertexBuffer;
class IndexBuffer;
class VertexArray;
class Texture;
class Shader;

struct ModelData
{
	std::vector<float> Vertices;
	std::vector<unsigned int> Indices;
};

enum class ModelType
{
	PlanetPoint, PlanetSphere, PlanetSOI, DV, SV, CV, Fighter, Skybox
};

class Model
{
public:
	Model(const std::string& filePath);
	Model(std::vector<float> vertices, std::vector<unsigned int> indices);
	~Model();

	void Bind();
	void Unbind();
	void InitGLBuffers();

	const ModelData& GetModelData() const { return m_ModelData; }

	const std::shared_ptr<Shader>& GetShader() const { return m_Shader; }
	void SetShader(std::shared_ptr<Shader> shader) { m_Shader = shader; InitGLBuffers(); }
	void AddTexture(std::shared_ptr<Texture> texture);
	const std::shared_ptr<VertexArray>& GetVAO() const { return m_VAO; }
	const std::shared_ptr<VertexBuffer>& GetVBO() const { return m_VBO; }
	const std::shared_ptr<IndexBuffer>& GetIBO() const { return m_IBO; }

private:
	void LoadFromOBJ(const std::string& filePath);

	ModelData m_ModelData;

	std::shared_ptr<Shader> m_Shader;
	std::vector<std::shared_ptr<Texture>> m_Textures;

	std::shared_ptr<VertexArray> m_VAO;
	std::shared_ptr<VertexBuffer> m_VBO;
	std::shared_ptr<IndexBuffer> m_IBO;
};
