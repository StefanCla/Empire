#include "Headers/MenuStates/MenuStartState.h"
#include "Headers/GameContext.h"

#ifdef LINUX
#include "../Empire_Linux/Headers/RenderWindow.h"
#elif WINDOWS
#include "../Empire_Windows/Headers/RenderWindow.h"
#endif

MenuStartState::MenuStartState() :
	m_Flags(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground)
{
	m_Name = "MenuStartState";
}

MenuStartState::~MenuStartState()
{}

void MenuStartState::OnStart()
{
	printf("MenuStartState has been started!");
}

void MenuStartState::OnUpdate()
{
	glm::vec2 windowSize = RenderWindow::Instance()->GetWindowSize();

	ImGui::SetNextWindowPos(ImVec2(windowSize.x * 0.5f - 37, windowSize.y * 0.5f - 120));
	ImGui::SetNextWindowSize(ImVec2(100, 50));
	ImGui::Begin("Menu Text", NULL, m_Flags);

	ImGui::SetWindowFontScale(1.8f);
	ImGui::Text("Empire");

	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(windowSize.x * 0.5f - 50, windowSize.y * 0.5f - 60));
	ImGui::SetNextWindowSize(ImVec2(150, 150));
	ImGui::Begin("Menu", NULL, m_Flags);

	if (ImGui::Button("Start", ImVec2(100, 60)))
	{
		m_Owner->ChangeState(m_Owner->GetState("MenuGenerateState"));
	}

	if (ImGui::Button("Exit", ImVec2(100, 60)))
	{
		GameContext::Instance()->SetCloseProgram(true);
	}

	ImGui::End();
}