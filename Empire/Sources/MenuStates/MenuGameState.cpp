#include "Headers/MenuStates/MenuGameState.h"
#include "Headers/Camera.h"
#include "Headers/Controllers/EmpireController.h"
#include "Headers/GameObjects/CarrierVessel.h"
#include <sstream>

#ifdef LINUX
#include "../Empire_Linux/Headers/RenderWindow.h"
#elif WINDOWS
#include "../Empire_Windows/Headers/RenderWindow.h"
#endif

void PushUIFlag(bool value)
{
	if (!value)
	{
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
	}
}

void PopUIFlag(bool value)
{
	if (!value)
	{
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}
}

MenuGameState::MenuGameState() :
	m_Selected(-1),
	m_Flags(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove),
	m_CVBuyMenuSwitch(false)
{
	m_Name = "MenuGameState";
}

MenuGameState::~MenuGameState()
{}

void MenuGameState::OnStart()
{
	printf("MenuGameState has been started!\n");
}

void MenuGameState::OnUpdate()
{
	glm::vec2 windowSize = RenderWindow::Instance()->GetWindowSize();
	EmpireController* player = GameContext::Instance()->GetPlayerEmpire();

	//Discovery Vessel Window
	ImGui::SetNextWindowPos(ImVec2(4, 135));
	ImGui::SetNextWindowSize(ImVec2(147, 110));
	ImGui::Begin("Discovery Vessels", NULL, m_Flags);
	for (int i = 0; i < player->GetShips()[ObjectType::DV].size(); i++)
	{
		if (player->GetShips()[ObjectType::DV][i]->GetActive())
		{
			sprintf(m_Buffer, "DV #%i", i + 1);

			if (ImGui::Selectable(m_Buffer, m_Selected == i))
			{
				if (m_Selected == i)
				{
					m_Selected = -1;
					GameContext::Instance()->SetSelectedShip(nullptr);
				}
				else
				{
					m_Selected = i;
					GameContext::Instance()->SetSelectedShip(player->GetShips()[ObjectType::DV][i]);
				}
			}
		}
		else
		{
			sprintf(m_Buffer, "Buy DV #%i", i + 1);

			bool ablePurchase = player->GetCredits() >= SHIP_DV_COST;
			PushUIFlag(ablePurchase);
			if (ImGui::Selectable(m_Buffer, m_Selected == i))
			{
				player->BuyShip(ObjectType::DV, SHIP_DV_COST);
			}
			PopUIFlag(ablePurchase);
		}
	}
	ImGui::End();

	//Supply Vessel Window
	ImGui::SetNextWindowPos(ImVec2(4, 247));
	ImGui::SetNextWindowSize(ImVec2(147, 110));
	ImGui::Begin("Supply Vessels", NULL, m_Flags);
	for (int i = 0; i < player->GetShips()[ObjectType::SV].size(); i++)
	{
		if (player->GetShips()[ObjectType::SV][i]->GetActive())
		{
			sprintf(m_Buffer, "SV #%i", i + 1);

			if (ImGui::Selectable(m_Buffer, m_Selected == i + SHIPNUM_DV))
			{
				if (m_Selected == i + SHIPNUM_DV)
				{
					m_Selected = -1;
					GameContext::Instance()->SetSelectedShip(nullptr);
				}
				else
				{
					m_Selected = i + SHIPNUM_DV;
					GameContext::Instance()->SetSelectedShip(player->GetShips()[ObjectType::SV][i]);
				}
			}
		}
		else
		{
			sprintf(m_Buffer, "Buy SV #%i", i + 1);

			bool ablePurchase = player->GetCredits() >= SHIP_SV_COST;
			PushUIFlag(ablePurchase);
			if (ImGui::Selectable(m_Buffer, m_Selected == i + SHIPNUM_DV))
			{
				player->BuyShip(ObjectType::SV, SHIP_SV_COST);
			}
			PopUIFlag(ablePurchase);
		}
	}
	ImGui::End();

	//Carrier Vessel Window
	ImGui::SetNextWindowPos(ImVec2(4, 359));
	ImGui::SetNextWindowSize(ImVec2(147, 224));
	ImGui::Begin("Carrier Vessels", NULL, m_Flags | ImGuiWindowFlags_HorizontalScrollbar);
	for (int i = 0; i < player->GetShips()[ObjectType::CV].size(); i++)
	{
		if (player->GetShips()[ObjectType::CV][i]->GetActive())
		{
			sprintf(m_Buffer, "CV #%i", i + 1);

			if (ImGui::Selectable(m_Buffer, m_Selected == i + (SHIPNUM_DV + SHIPNUM_SV)))
			{
				if (m_Selected == i + (SHIPNUM_DV + SHIPNUM_SV))
				{
					m_Selected = -1;
					GameContext::Instance()->SetSelectedShip(nullptr);
				}
				else
				{
					m_Selected = i + (SHIPNUM_DV + SHIPNUM_SV);
					GameContext::Instance()->SetSelectedShip(player->GetShips()[ObjectType::CV][i]);
				}
			}
		}
	}
	ImGui::End();

	//Hover information
	GameObject* hoveredObject = GameContext::Instance()->GetHoverObject();
	if (hoveredObject != nullptr)
	{
		ImGui::SetNextWindowPos(ImVec2(4, 5));
		ImGui::SetNextWindowSize(ImVec2(147, 128));
		ImGui::Begin("Info: ", NULL, m_Flags);

		DisplayData(hoveredObject);

		ImGui::End();
	}

	//Selected planet
	GameObject* selectedObject = GameContext::Instance()->GetSelectedObject();
	if (selectedObject != nullptr)
	{
		ImGui::SetNextWindowPos(ImVec2(windowSize.x - 144, windowSize.y - 177 - 42));
		ImGui::SetNextWindowSize(ImVec2(142, 180));
		ImGui::Begin("Selected Info: ", NULL, m_Flags);

		DisplayData(selectedObject);
		if (selectedObject->GetObjectType() == ObjectType::Planet)
		{
			Planet* planet = static_cast<Planet*>(selectedObject);
			
			if (planet->GetOwner() == player)
			{
				bool canBuyCarrier = (player->GetShipTypeCount(ObjectType::CV) < SHIPNUM_CV) && (planet->GetTechLevel() == 15) && (player->GetCredits() >= SHIP_CV_COST);
				PushUIFlag(canBuyCarrier);
				if (ImGui::Button("Build Carrier"))
				{
					if (m_CVBuyMenuSwitch)
						m_CVBuyMenuSwitch = false;
					else
						m_CVBuyMenuSwitch = true;
				}
				PopUIFlag(canBuyCarrier);

				bool canIncreaseTechLevel = (!planet->IsMaxTechLevel()) && (planet->GetTechLevelIncreaseCost() <= planet->GetOwner()->GetCredits());
				std::stringstream ss;
				ss << "Increase tech (" << planet->GetTechLevelIncreaseCost() << ")";

				PushUIFlag(canIncreaseTechLevel);
				if (ImGui::Button(ss.str().c_str()))
				{
					planet->IncreaseTechLevel();
				}
				PopUIFlag(canIncreaseTechLevel);
			}

			//Only show this window when the Buy CV button has been pressed
			if (m_CVBuyMenuSwitch)
			{
				ImGui::SetNextWindowPos(ImVec2(windowSize.x - 294, windowSize.y - 177 - 16));
				ImGui::SetNextWindowSize(ImVec2(142, 153));
				ImGui::Begin("CV Size: ", NULL, m_Flags);

				if (ImGui::Button("Size 15", ImVec2(125, 35)))
				{
					player->BuyShip(ObjectType::CV, SHIP_CV_COST, 15);
					m_CVBuyMenuSwitch = false;
				}
				else if (ImGui::Button("Size 20", ImVec2(125, 35)))
				{
					player->BuyShip(ObjectType::CV, SHIP_CV_COST, 20);
					m_CVBuyMenuSwitch = false;
				}
				else if (ImGui::Button("Size 25", ImVec2(125, 35)))
				{
					player->BuyShip(ObjectType::CV, SHIP_CV_COST, 25);
					m_CVBuyMenuSwitch = false;
				}

				ImGui::End();
			}
		}

		ImGui::End();
	}
	
	//Movement window
	if (GameContext::Instance()->GetSelectedShip() != nullptr && GameContext::Instance()->GetSelectedObject() != nullptr && GameContext::Instance()->GetSelectedObject()->GetObjectType() == ObjectType::Planet)
	{
		ImGui::SetNextWindowPos(ImVec2(4, windowSize.y - 92));
		ImGui::SetNextWindowSize(ImVec2(147, 38));;
		ImGui::Begin("Move Button", NULL, m_Flags | ImGuiWindowFlags_NoTitleBar);

		if (ImGui::Button("Move Ship", ImVec2(128, 22)))
		{
			Planet* planet = static_cast<Planet*>(GameContext::Instance()->GetSelectedObject());
			GameContext::Instance()->GetSelectedShip()->SetMove(planet);
		}

		ImGui::End();
	}

	//Empire information
	ImGui::SetNextWindowPos(ImVec2(4, windowSize.y - 52));
	ImGui::SetNextWindowSize(ImVec2(147, 52));;
	ImGui::Begin("Empire Info: ", NULL, m_Flags);

	ImGui::Text("Credits: %i", player->GetCredits());

	ImGui::End();

	//End turn button
	ImGui::SetNextWindowPos(ImVec2(windowSize.x - 128 - 16, windowSize.y - 24 - 16));
	ImGui::Begin("Controls", NULL, m_Flags | ImGuiWindowFlags_NoTitleBar);

	bool isWaiting = GameContext::Instance()->IsWaitingForPlayerTurn();

	PushUIFlag(isWaiting);
	if (ImGui::Button("End turn", ImVec2(128, 24)))
		GameContext::Instance()->NotifyTurnThread();
	PopUIFlag(isWaiting);

	ImGui::End();
}

//Cast object to their correct type and display info
void MenuGameState::DisplayData(GameObject* object)
{
	switch (object->GetObjectType())
	{
	case ObjectType::DV:
	{
		ImGui::Text("DV data");
	}
	break;
	case ObjectType::SV:
	{
		ImGui::Text("SV data");
	}
	break;
	case ObjectType::CV:
	{
		CarrierVessel* cv = static_cast<CarrierVessel*>(object);
		ImGui::Text("Size: %i", cv->GetSize());
	}
	break;
	case ObjectType::Planet:
	{
		Planet* planet = static_cast<Planet*>(object);
		ImGui::Text(planet->GetName().c_str());
		ImGui::Text("Resource level: %i", planet->GetResourceLevel());
		ImGui::Text("Tech level: %i", planet->GetTechLevel());
		ImGui::Text("Credits: %i", planet->GetCredits());
		ImGui::Text("Fighters: %i", planet->GetFighters());
	}
	break;
	}
}