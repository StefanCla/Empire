#include "Headers/GameObjects/Planet.h"
#include "Headers/Models/ModelManager.h"
#include "Headers/Textures/TextureManager.h"
#include "Headers/Random.h"
#include "Headers/Controllers/EmpireController.h"
#include <bullet/btBulletDynamicsCommon.h>

Planet::Planet()
{
	m_TechLevel = Random::Instance()->IntRangePeakMiddle(1, 15);
	m_ResourceLevel = Random::Instance()->IntRangePeakMiddle(1, 5);
	m_Credits = 0;
	m_Fighters = 0;
	m_Color = glm::vec3(1.0f, 1.0f, 1.0f);

	m_Type = ObjectType::Planet;
	m_Model = ModelManager::Instance()->GetModel(ModelType::PlanetSphere);

	InitMatrices();
	Rotate({ 20.0f, 0.0f, 0.0f });
	Scale({ 0.25f, 0.25f, 0.25f });

	//FSM
	m_GameStateHandler = new FSM(this);
	m_ActiveState = new PlanetActiveState();
	m_ConversionState = new PlanetConversionState();
	m_InactiveState = new PlanetInactiveState();

	m_GameStateHandler->RegisterState(m_ActiveState);
	m_GameStateHandler->RegisterState(m_ConversionState);
	m_GameStateHandler->RegisterState(m_InactiveState);

	m_GameStateHandler->FirstState(m_InactiveState);
}

Planet::~Planet()
{
}

void Planet::Update(float dt)
{
	//glm::vec3 rotation = glm::vec3(0.0f, -15.0f, 0.0f) * dt;
	//Rotate(rotation);
}

void Planet::OnTurnStart()
{
	// This function is called at the beginning of the owner's turn
	// Generate credits and other resources
	m_GameStateHandler->Update();
}

void Planet::InitBullet()
{
	m_BoxCollision = new btBoxShape(btVector3(m_Scales.x, m_Scales.y, m_Scales.z));
	GameObject::InitBullet();
}

void Planet::SetActiveState()
{
	m_GameStateHandler->FirstState(m_ActiveState);
}

void Planet::ConvertPlanet()
{
	m_Color = m_Owner->GetColor();
}

void Planet::GenerateCredits()
{
	float generated = PLANET_BASE_CREDIT_GEN * m_ResourceLevel * (m_TechLevel * PLANET_CREDITS_TECH_LEVEL_WEIGHT);
	m_Credits += generated;
	m_Owner->AddCredits(generated * PLANET_MEMBERSHIP_FEE_WEIGHT);
}

float Planet::GetSOIRadius() const
{
	return PLANET_BASE_SOI_RADIUS + m_TechLevel * PLANET_SOI_TECH_LEVEL_WEIGHT;
}

float Planet::GetMaxSOIRadius() const
{
	return PLANET_BASE_SOI_RADIUS + 15 * PLANET_SOI_TECH_LEVEL_WEIGHT;
}

bool Planet::IsMaxTechLevel() const
{
	return m_TechLevel == 15 ? true : false;
}

unsigned int Planet::GetTechLevelIncreaseCost() const
{
	return m_TechLevel * PLANET_TECH_LEVEL_INCREASE_COST;
}

void Planet::IncreaseTechLevel()
{
	m_Owner->RemoveCredits(GetTechLevelIncreaseCost());
	m_TechLevel++;
}
