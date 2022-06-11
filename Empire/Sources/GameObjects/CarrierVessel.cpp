#include "Headers/GameObjects/CarrierVessel.h"
#include "Headers/Models/ModelManager.h"
#include "Headers/Textures/TextureManager.h"
#include "Headers/GameObjects/Planet.h"

CarrierVessel::CarrierVessel() :
	m_Size(0)
{
	m_Type = ObjectType::CV;
	m_Model = ModelManager::Instance()->GetModel(ModelType::CV);

	InitMatrices();
	Rotate({ 20.0f, 0.0f, 0.0f });
}

CarrierVessel::~CarrierVessel()
{}

void CarrierVessel::Update(float deltaTime)
{
	Move(deltaTime);

	glm::vec3 rotation = glm::vec3(0.0f, -15.0f, 0.0f) * deltaTime;
	Rotate(rotation);
}

void CarrierVessel::ExecuteObjective()
{
	printf("I reached the end but i don't do anything yet!\n");
}