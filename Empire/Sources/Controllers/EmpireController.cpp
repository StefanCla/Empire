#include "Headers/Controllers/EmpireController.h"
#include "Headers/PlanetManager.h"
#include "Headers/GameObjects/Planet.h"
#include "Headers/GameObjects/Vessel.h"
#include "Headers/Models/ModelManager.h"
#include "Headers/Renderer/Renderer.h"
#include "Headers/Renderer/VertexArray.h"
#include "Headers/Renderer/VertexBuffer.h"
#include "Headers/GameObjects/CarrierVessel.h"
#include <glm/gtc/type_ptr.hpp>

EmpireController::EmpireController(const glm::vec3& color)
	: m_Color(color)
{
	std::shared_ptr<Planet> homePlanet = PlanetManager::Instance()->GetRandomHomePlanet();
	homePlanet->SetOwner(this);
	homePlanet->SetColor(color);
	homePlanet->SetActiveState();

	m_Planets.insert({ homePlanet->GetName(), homePlanet });
}

EmpireController::~EmpireController()
{
}

void EmpireController::RenderSOI()
{
	const std::shared_ptr<Model>& soiModel = ModelManager::Instance()->GetModel(ModelType::PlanetSOI);
	soiModel->Bind();

	std::vector<float> planetRadii;
	std::vector<glm::vec3> planetPositions;

	std::unordered_map<std::string, std::shared_ptr<Planet>>::const_iterator it;
	for (it = m_Planets.begin(); it != m_Planets.end(); it++)
	{
		planetRadii.push_back(it->second->GetSOIRadius());
		planetPositions.push_back(it->second->GetPosition());
	}

	soiModel->GetVAO()->GetVertexBuffer(VertexBufferType::RadiusVB)->SetBufferData(&planetRadii[0], planetRadii.size() * sizeof(float));
	soiModel->GetVAO()->GetVertexBuffer(VertexBufferType::PositionVB)->SetBufferData(glm::value_ptr(planetPositions[0]), planetPositions.size() * sizeof(glm::vec3));

	Renderer::Instance()->Submit(soiModel->GetShader());
	Renderer::Instance()->RenderCirclesInstanced(soiModel->GetShader(), m_Color, soiModel->GetModelData().Vertices.size() / 5, planetRadii.size());
}

void EmpireController::AddPlanet(std::shared_ptr<Planet> planet)
{
	planet->SetOwner(this);
	planet->SetColor(m_Color);

	m_Planets.insert({ planet->GetName(), planet });
}

void EmpireController::AddPlanetByName(const std::string& planetName)
{
	std::shared_ptr<Planet> planet = PlanetManager::Instance()->GetPlanets()[planetName];
	AddPlanet(planet);
}

void EmpireController::AddShip(ObjectType type, std::vector<std::shared_ptr<Vessel>> ship)
{
	std::vector<std::shared_ptr<Vessel>>::iterator it;
	for (it = ship.begin(); it != ship.end(); it++)
		it->get()->SetOwner(this);

	m_Ships.insert({ type, ship });
}

unsigned int EmpireController::GetShipTypeCount(ObjectType type) const
{
	int count = 0;
	for (const std::shared_ptr<Vessel>& vessel : m_Ships.at(type))
		if (vessel->GetActive())
			count++;

	return count;
}

void EmpireController::BuyShip(ObjectType type, unsigned int cost, unsigned int size)
{
	RemoveCredits(cost);
	int index = GetShipTypeCount(type);
	m_Ships.at(type)[index]->SetActive(true);

	if (type == ObjectType::CV)
	{
		std::shared_ptr<CarrierVessel> carrier = std::static_pointer_cast<CarrierVessel>(m_Ships.at(type)[index]);
		carrier->SetSize(size);
	}
}
