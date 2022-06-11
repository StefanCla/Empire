#include "Headers/PlanetManager.h"
#include "Headers/GameContext.h"
#include "Headers/Camera.h"
#include "Headers/GameObjects/Planet.h"
#include "Headers/Renderer/Renderer.h"
#include "Headers/Renderer/VertexArray.h"
#include "Headers/Renderer/VertexBuffer.h"
#include "Headers/Renderer/IndexBuffer.h"
#include "Headers/Models/ModelManager.h"
#include <glm/gtc/type_ptr.hpp>
#include <bullet/btBulletDynamicsCommon.h>

PlanetManager* PlanetManager::Instance()
{
	static PlanetManager* s_Instance = new PlanetManager;
	return s_Instance;
}

PlanetManager::PlanetManager()
{
}

PlanetManager::~PlanetManager()
{
}

void PlanetManager::Init(btDiscreteDynamicsWorld* staticWorld)
{
	std::shared_ptr<VertexBuffer> m_PlanetColorsVB = std::make_shared<VertexBuffer>(PLANETNUMBER * sizeof(glm::vec3));
	m_PlanetColorsVB->SetLayout({
		{ ShaderDataType::Float3, "in_instanceColor" }
		});

	std::shared_ptr<VertexBuffer> m_PlanetPositionsVB = std::make_shared<VertexBuffer>(PLANETNUMBER * sizeof(glm::vec3));
	m_PlanetPositionsVB->SetLayout({
		{ ShaderDataType::Float3, "in_instancePosition" }
		});

	std::shared_ptr<VertexBuffer> m_PlanetSOIRadiiVB = std::make_shared<VertexBuffer>(PLANETNUMBER * sizeof(float));
	m_PlanetSOIRadiiVB->SetLayout({
		{ ShaderDataType::Float, "in_instanceRadius" }
		});

	std::shared_ptr<VertexBuffer> m_PlanetMatricesVB = std::make_shared<VertexBuffer>(PLANETNUMBER * sizeof(glm::mat4));
	m_PlanetMatricesVB->SetLayout({
		{ ShaderDataType::Mat4, "in_instanceTransform" }
		});

	std::unordered_map<std::string, std::shared_ptr<Planet>>::iterator it;
	for (it = m_Planets.begin(); it != m_Planets.end(); it++)
	{
		m_PlanetColors.push_back(it->second->GetColor());
		m_PlanetPositions.push_back(it->second->GetPosition());
		m_PlanetSOIRadii.push_back(it->second->GetSOIRadius());
		m_PlanetMatrices.push_back(it->second->GetModelMatrix());
		staticWorld->addRigidBody(it->second->GetRigidBody());
	}

	m_PlanetColorsVB->SetBufferData(glm::value_ptr(m_PlanetColors[0]), m_PlanetColors.size() * sizeof(glm::vec3));
	m_PlanetPositionsVB->SetBufferData(glm::value_ptr(m_PlanetPositions[0]), m_PlanetPositions.size() * sizeof(glm::vec3));
	m_PlanetSOIRadiiVB->SetBufferData(&m_PlanetSOIRadii[0], m_PlanetSOIRadii.size() * sizeof(float));
	m_PlanetMatricesVB->SetBufferData(glm::value_ptr(m_PlanetMatrices[0][0]), m_PlanetMatrices.size() * sizeof(glm::mat4));

	const std::shared_ptr<Model>& pointModel = ModelManager::Instance()->GetModel(ModelType::PlanetPoint);
	pointModel->GetVAO()->AddVertexBuffer(VertexBufferType::ColorVB, pointModel->GetShader()->GetRendererID(), m_PlanetColorsVB);
	pointModel->GetVAO()->AddVertexBuffer(VertexBufferType::PositionVB, pointModel->GetShader()->GetRendererID(), m_PlanetPositionsVB);

	const std::shared_ptr<Model>& soiModel = ModelManager::Instance()->GetModel(ModelType::PlanetSOI);
	soiModel->GetVAO()->AddVertexBuffer(VertexBufferType::ColorVB, soiModel->GetShader()->GetRendererID(), m_PlanetColorsVB);
	soiModel->GetVAO()->AddVertexBuffer(VertexBufferType::RadiusVB, soiModel->GetShader()->GetRendererID(), m_PlanetSOIRadiiVB);
	soiModel->GetVAO()->AddVertexBuffer(VertexBufferType::PositionVB, soiModel->GetShader()->GetRendererID(), m_PlanetPositionsVB);

	const std::shared_ptr<Model>& planetModel = ModelManager::Instance()->GetModel(ModelType::PlanetSphere);
	planetModel->GetVAO()->AddVertexBuffer(VertexBufferType::ModelMatrixVB, planetModel->GetShader()->GetRendererID(), m_PlanetMatricesVB);

	m_Active = true;
		
	// Temporary code for rendering ship model
	//const std::shared_ptr<Model>& cvModel = ModelManager::Instance()->GetModel(ModelType::CV);
	//cvModel->GetVAO()->AddVertexBuffer(VertexBufferType::ModelMatrixVB, cvModel->GetShader()->GetRendererID(), m_PlanetMatricesVB);
}

void PlanetManager::Update(float deltaTime)
{
	std::unordered_map<std::string, std::shared_ptr<Planet>>::iterator it;
	for (it = m_Planets.begin(); it != m_Planets.end(); it++)
		it->second->Update(deltaTime);

	// Color instance data only has to be updated for rendering planet points / planet SOI
	// Radii instance data only has to be updated for rendering planet SOI
	// Transform instance data only has to be updated for rendering planet spheres
	UpdateColorInstanceData();
	UpdateSOIRaddiInstanceData();
	//UpdateTransformInstanceData();
}

void PlanetManager::Render()
{
	// Draw planets as points
	/*const std::shared_ptr<Model>& planetPointModel = ModelManager::Instance()->GetModel(ModelType::PlanetPoint);
	planetPointModel->Bind();

	planetPointModel->GetVAO()->GetVertexBuffer(VertexBufferType::ColorVB)->SetBufferData(glm::value_ptr(m_PlanetColors[0]), m_PlanetColors.size() * sizeof(glm::vec3));
	Renderer::Instance()->Submit(planetPointModel->GetShader());
	Renderer::Instance()->RenderPointsInstanced(planetPointModel->GetShader(), m_Planets.size());*/

	// Draw planets as textured Spheres
	const std::shared_ptr<Model>& planetSphereModel = ModelManager::Instance()->GetModel(ModelType::PlanetSphere);
	planetSphereModel->Bind();

	planetSphereModel->GetVAO()->GetVertexBuffer(VertexBufferType::ModelMatrixVB)->SetBufferData(glm::value_ptr(m_PlanetMatrices[0][0]), m_PlanetMatrices.size() * sizeof(glm::mat4));
	Renderer::Instance()->Submit(planetSphereModel->GetShader());
	Renderer::Instance()->RenderElementsInstanced(planetSphereModel->GetShader(), planetSphereModel->GetIBO()->GetCount(), m_Planets.size(), false);
}

void PlanetManager::UpdateColorInstanceData()
{
	m_PlanetColors.clear();
	std::unordered_map<std::string, std::shared_ptr<Planet>>::iterator it;

	for (it = m_Planets.begin(); it != m_Planets.end(); it++)
		m_PlanetColors.push_back(it->second->GetColor());
}

void PlanetManager::UpdateSOIRaddiInstanceData()
{
	m_PlanetSOIRadii.clear();
	std::unordered_map<std::string, std::shared_ptr<Planet>>::iterator it;

	for (it = m_Planets.begin(); it != m_Planets.end(); it++)
		m_PlanetSOIRadii.push_back(it->second->GetSOIRadius());
}

void PlanetManager::UpdateTransformInstanceData()
{
	m_PlanetMatrices.clear();
	std::unordered_map<std::string, std::shared_ptr<Planet>>::iterator it;

	for (it = m_Planets.begin(); it != m_Planets.end(); it++)
		m_PlanetMatrices.push_back(it->second->GetModelMatrix());
}

std::unordered_map<std::string, std::shared_ptr<Planet>>& PlanetManager::GetPlanets()
{
	return m_Planets;
}

std::shared_ptr<Planet> PlanetManager::GetRandomHomePlanet()
{
	auto random = std::next(std::begin(m_Planets), Random::Instance()->IntRange(0, m_Planets.size()));
	while (!random->second->IsNeutral())
		random = std::next(std::begin(m_Planets), Random::Instance()->IntRange(0, m_Planets.size()));

	return random->second;
}
