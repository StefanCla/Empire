#include "Headers/GameObjects/SupplyVessel.h"
#include "Headers/Models/ModelManager.h"
#include "Headers/Textures/TextureManager.h"
#include "Headers/GameObjects/Planet.h"

SupplyVessel::SupplyVessel() :
	m_Credits(0)
{
	m_Type = ObjectType::SV;
	m_Model = ModelManager::Instance()->GetModel(ModelType::SV);

	InitMatrices();
	Rotate({ 20.0f, 0.0f, 0.0f });
}

SupplyVessel::~SupplyVessel()
{}

void SupplyVessel::Update(float deltaTime)
{
	Move(deltaTime);

	glm::vec3 rotation = glm::vec3(0.0f, -15.0f, 0.0f) * deltaTime;
	Rotate(rotation);

	//Check if at home planet. 
	//If true, calculate path to 10 richest planets in your possession
	//If false, follow the line to these planets
		//If at those planets, gather credits from the planets

}

void SupplyVessel::ExecuteObjective()
{
	printf("I reached the end but i don't do anything yet!\n");
}

