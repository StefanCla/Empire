#include "Headers/PlanetStates/PlanetActiveState.h"
#include "Headers/Controllers/EmpireController.h"

PlanetActiveState::PlanetActiveState()
{
	m_Name = "PlanetActiveState";
}

PlanetActiveState::~PlanetActiveState()
{}

void PlanetActiveState::OnStart()
{
}

void PlanetActiveState::OnUpdate()
{
	UpdateCredits();
	UpdateSOI();
}

void PlanetActiveState::UpdateCredits()
{
	Planet* planet = static_cast<Planet*>(m_Owner->GetOwner());
	planet->GenerateCredits();
}

void PlanetActiveState::UpdateSOI()
{
	Planet* planet = static_cast<Planet*>(m_Owner->GetOwner());

	for (size_t i = 0; i < planet->GetSphereList().size(); i++)
	{
		float distance = glm::distance(planet->GetPosition(), planet->GetSphereList()[i]->GetPosition());

		if (planet->GetSOIRadius() >= distance)
		{
			std::vector<std::shared_ptr<Planet>>& sphereList = planet->GetSphereList();

			if (sphereList[i]->IsNeutral())//is neuteral
			{
				//change planet state to conversion
				sphereList[i]->m_GameStateHandler->ChangeState(
					sphereList[i]->m_GameStateHandler->GetState("PlanetConversionState"));

				sphereList[i]->GetConversionState()->SetTurns(5);
				planet->GetOwner()->AddPlanet(sphereList[i]);
			}

			//PoP item from m_SphereList //NOTE: Might cause problems since the size will also change, thus the loop might fuck up
			sphereList[i] = sphereList[sphereList.size() - 1];
			sphereList.pop_back();
			i--;
			planet->SetSphereList(sphereList);
		}
	}
}
