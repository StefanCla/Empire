#include "Headers/GameObjects/DiscoveryVessel.h"
#include "Headers/Models/ModelManager.h"
#include "Headers/Textures/TextureManager.h"
#include "Headers/GameObjects/Planet.h"
#include "Headers/Controllers/EmpireController.h"

DiscoveryVessel::DiscoveryVessel()
{
	m_Type = ObjectType::DV;
	m_Model = ModelManager::Instance()->GetModel(ModelType::DV);

	InitMatrices();
	Rotate({ 20.0f, 0.0f, 0.0f });
}

DiscoveryVessel::~DiscoveryVessel()
{}

void DiscoveryVessel::Update(float deltaTime)
{
	Move(deltaTime);

	//glm::vec3 rotation = glm::vec3(0.0f, -15.0f, 0.0f) * deltaTime;
	//Rotate(rotation);
}

void DiscoveryVessel::ExecuteObjective()
{
	if (m_Planet->IsNeutral())
	{
		m_Planet->m_GameStateHandler->ChangeState(m_Planet->m_GameStateHandler->GetState("PlanetConversionState"));
		m_Planet->GetConversionState()->SetTurns(3);
		m_Owner->AddPlanetByName(m_Planet->GetName());
	}
}