#pragma once
#include "Headers/Precomp.h"
#include <memory>

class Shader;
class Model;
class Camera;
class VertexArray;

class Renderer
{
public:
	Renderer(const Renderer& r) = delete;
	Renderer& operator=(const Renderer& r) = delete;

	static Renderer* Instance();

	void Init(unsigned int width, unsigned int height);
	void BeginScene(Camera* camera);
	void Submit(const std::shared_ptr<Shader>& shader);
	
	void RenderSkybox(const std::shared_ptr<Shader>& shader, unsigned int indexCount);

	void RenderElementsInstanced(const std::shared_ptr<Shader>& shader, unsigned int indexCount, unsigned int instances, bool emissiveTex = true);
	void RenderPointsInstanced(const std::shared_ptr<Shader>& shader, unsigned int instances);
	void RenderCirclesInstanced(const std::shared_ptr<Shader>& shader, const glm::vec3& color, unsigned int vertexCount, unsigned int instances);

	void Clear();
	void SetClearColor(const glm::vec4& color);
	void SetViewProjection(const glm::mat4& viewProjection);
	void ResizeViewport(unsigned int width, unsigned int height);

private:
	Renderer();
	~Renderer();

	struct SceneData
	{
		glm::vec3 ViewPosition;
		glm::mat4 ViewMatrix;
		glm::mat4 ProjectionMatrix;
		glm::mat4 ViewProjectionMatrix;
	};

	SceneData* m_SceneData;
};
