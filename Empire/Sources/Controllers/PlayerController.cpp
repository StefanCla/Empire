#include "Headers/Controllers/PlayerController.h"
#include "Headers/GameObjects/Planet.h"
#include "Headers/GameContext.h"
#include "Headers/GameObjects/DiscoveryVessel.h"

PlayerController::PlayerController(const glm::vec3& color)
	: EmpireController(color)
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::FirstTurn()
{
	m_Ships[ObjectType::DV][0]->SetActive(true);
}

void PlayerController::StartTurn()
{
	printf("Player has started his turn\n");
	printf("Updating player planets\n");

	std::unordered_map<std::string, std::shared_ptr<Planet>>::iterator it;
	for (it = m_Planets.begin(); it != m_Planets.end(); it++)
		it->second->OnTurnStart();

	//Loop every turn for this
	std::unordered_map<ObjectType, std::vector<std::shared_ptr<Vessel>>>::iterator itz;
	for (itz = m_Ships.begin(); itz != m_Ships.end(); itz++)
	{
		for (std::shared_ptr<Vessel> ship : itz->second)
		{
			if(ship->GetActive())
				ship->SetSmallDist();
		}
	}

	GameContext::Instance()->WaitForPlayerTurnEnd();
}
