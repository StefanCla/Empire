#include "Headers/Renderer/Renderer.h"
#include "Headers/Renderer/Shader.h"
#include "Headers/Renderer/VertexArray.h"
#include "Headers/Renderer/VertexBuffer.h"
#include "Headers/Renderer/IndexBuffer.h"
#include "Headers/Models/Model.h"
#include "Headers/Camera.h"
#include "Headers/Textures/Texture.h"
#include <glm/gtc/type_ptr.hpp>

Renderer* Renderer::Instance()
{
	static Renderer* s_Instance = new Renderer();
	return s_Instance;
}

Renderer::Renderer()
{
	// Init scene data
	m_SceneData = new SceneData();
}

Renderer::~Renderer()
{
	// Delete scene data
	delete m_SceneData;
}

void Renderer::Init(unsigned int width, unsigned int height)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

#ifdef WINDOWS
	glDepthRange(0.0f, 1.0f);
	glClearDepth(1.0f);
#elif LINUX
	glDepthRangef(0.0f, 1.0f);
	glClearDepthf(1.0f);
#endif

	int t;
	printf("This GPU supplied by: %s\n", glGetString(GL_VENDOR));
	printf("This GPU supports: %s\n", glGetString(GL_VERSION));
	printf("This GPU Renders with: %s\n", glGetString(GL_RENDERER));
	printf("This GPU supports: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &t);
	printf("This GPU MaxTexSize is: %i\n", t);
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &t);
	printf("This GPU supports %i Texture units\n", t);

	glGetIntegerv(GL_NUM_EXTENSIONS, &t);
	printf("This GPU supports these extensions: \n");
	for (int i = 0; i < t; i++)
		printf("%s\n", glGetStringi(GL_EXTENSIONS, i));

	glViewport(0, 0, width, height);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	glCullFace(GL_BACK);

	if (glGetError() != GL_NO_ERROR)
		printf("Failed to initialize Renderer\n");
}

void Renderer::BeginScene(Camera* camera)
{
	m_SceneData->ViewPosition = camera->GetPosition();
	m_SceneData->ViewMatrix = glm::mat4(glm::mat3(camera->GetViewMatrix()));
	m_SceneData->ProjectionMatrix = camera->GetProjectionMatrix();
	m_SceneData->ViewProjectionMatrix = camera->GetViewProjectionMatrix();
}

void Renderer::Submit(const std::shared_ptr<Shader>& shader)
{
	shader->Bind();
	shader->SetUniformMat4("u_viewProj", m_SceneData->ViewProjectionMatrix);
	
	unsigned int viewPositionLoc = shader->GetUniformLocation("u_viewPos");
	glUniform3f(viewPositionLoc, m_SceneData->ViewPosition.x, m_SceneData->ViewPosition.y, m_SceneData->ViewPosition.z);
	
	float ambient[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	unsigned int ambientLoc = shader->GetUniformLocation("u_ambient");
	glUniform4fv(ambientLoc, 1, &ambient[0]);
}

void Renderer::RenderSkybox(const std::shared_ptr<Shader>& shader, unsigned int indexCount)
{
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);

	shader->SetUniformMat4("u_projection", m_SceneData->ProjectionMatrix);
	shader->SetUniformMat4("u_view", m_SceneData->ViewMatrix);
	
	unsigned int cubeSamplerLoc = shader->GetUniformLocation("u_cube");
	glUniform1i(cubeSamplerLoc, 0);

	glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr, 1);

	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
}

void Renderer::RenderElementsInstanced(const std::shared_ptr<Shader>& shader, unsigned int indexCount, unsigned int instances, bool emissiveTex)
{
	unsigned int instancedLoc = shader->GetUniformLocation("u_instanced");
	glUniform1i(instancedLoc, 1);

	unsigned int diffuseLoc = shader->GetUniformLocation("u_diffuse");
	glUniform1i(diffuseLoc, 0);

	if (emissiveTex)
	{
		unsigned int emissiveTexLoc = shader->GetUniformLocation("u_emissiveTex");
		glUniform1i(emissiveTexLoc, 1);

		unsigned int emissiveLoc = shader->GetUniformLocation("u_emissive");
		glUniform1i(emissiveLoc, 1);
	}

	glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr, instances);
	glUniform1i(instancedLoc, 0);

	unsigned int emissiveTexLoc = shader->GetUniformLocation("u_emissiveTex");
	glUniform1i(emissiveTexLoc, 0);
}

void Renderer::RenderPointsInstanced(const std::shared_ptr<Shader>& shader, unsigned int instances)
{
	unsigned int pointsLoc = shader->GetUniformLocation("u_points");
	glUniform1i(pointsLoc, 1);

	unsigned int pointSizeLoc = shader->GetUniformLocation("u_pointSize");
	glUniform1f(pointSizeLoc, 1.0f);

	glDrawArraysInstanced(GL_POINTS, 0, 1, instances);
	glUniform1i(pointsLoc, 0);
}

void Renderer::RenderCirclesInstanced(const std::shared_ptr<Shader>& shader, const glm::vec3& color, unsigned int vertexCount, unsigned int instances)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_CONSTANT_ALPHA);

	unsigned int circlesLoc = shader->GetUniformLocation("u_circles");
	glUniform1i(circlesLoc, 1);

	unsigned int circleColorLoc = shader->GetUniformLocation("u_circleColor");
	glUniform3f(circleColorLoc, color.x, color.y, color.z);

	glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, vertexCount, instances);
	glUniform1i(circlesLoc, 0);

	glDisable(GL_BLEND);
}

void Renderer::Clear()
{
	// Clear buffer to a preset value
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::SetClearColor(const glm::vec4& color)
{
	// Set the preset value which is used to clear the buffer
	glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::SetViewProjection(const glm::mat4& viewProjection)
{
	// Set the view projection matrix of the current scene
	m_SceneData->ViewProjectionMatrix = viewProjection;
}

void Renderer::ResizeViewport(unsigned int width, unsigned int height)
{
	glViewport(0, 0, width, height);
}
