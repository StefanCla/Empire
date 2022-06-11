#include "Headers/Controllers/AIController.h"
#include "Headers/GameObjects/Planet.h"
#include <algorithm>

AIController::AIController(const glm::vec3& color)
	: EmpireController(color)
{
}

AIController::~AIController()
{
}

void AIController::FirstTurn()
{}

void AIController::StartTurn()
{
	printf("AI has started his turn\n");
	printf("Updating AI planets\n");

	std::unordered_map<std::string, std::shared_ptr<Planet>>::iterator it;
	for (it = m_Planets.begin(); it != m_Planets.end(); it++)
		it->second->OnTurnStart();

	printf("AI credits: %i\n", m_Credits);
	//if (m_Credits > 10000)
	UpgradePlanets();
}

struct SortTechLevelDescending
{
	inline bool operator() (const std::shared_ptr<Planet>& first, const std::shared_ptr<Planet>& second)
	{
		return (first->GetTechLevel() > second->GetTechLevel());
	}
};

void AIController::UpgradePlanets()
{
	int checkingMinLevel = 5;
	bool upgradeCandidatesFound = false;
	std::vector<std::shared_ptr<Planet>> upgradeCandidates;

	while (checkingMinLevel > 2 && !upgradeCandidatesFound)
	{
		// Find possible upgrade candidates
		std::unordered_map<std::string, std::shared_ptr<Planet>>::iterator it;
		for (it = m_Planets.begin(); it != m_Planets.end(); it++)
		{
			if (it->second->GetResourceLevel() >= checkingMinLevel && !it->second->IsMaxTechLevel())
			{
				upgradeCandidates.push_back(it->second);
			}
		}

		// If no candidates were found, lower level to check for, else break out of loop and proceed
		if (upgradeCandidates.size() == 0)
			checkingMinLevel--;
		else
		{
			printf("AI found potential upgrade candidates: %i\n", upgradeCandidates.size());
			upgradeCandidatesFound = true;
		}
	}

	// Sort upgrade candidates from lowest tech to highest tech level
	std::sort(upgradeCandidates.begin(), upgradeCandidates.end(), SortTechLevelDescending());

	// Iterate through upgrade candidates
	std::vector<std::shared_ptr<Planet>>::iterator it;
	for (it = upgradeCandidates.begin(); it != upgradeCandidates.end(); it++)
	{
		// If AI can afford upgrade, do upgrade, else break because AI is only interested in upgrading the best planet possible
		if (it->get()->GetTechLevelIncreaseCost() <= m_Credits)
		{
			printf("AI has increased tech level of planet: %s\n", it->get()->GetName().c_str());
			it->get()->IncreaseTechLevel();
		}
		else
		{
			break;
		}
	}
}
