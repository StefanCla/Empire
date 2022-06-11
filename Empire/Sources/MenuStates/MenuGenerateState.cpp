#include "Headers/MenuStates/MenuGenerateState.h"

#ifdef LINUX
#include "../Empire_Linux/Headers/RenderWindow.h"
#elif WINDOWS
#include "../Empire_Windows/Headers/RenderWindow.h"
#endif

MenuGenerateState::MenuGenerateState() :
	arms(1),
	armOffset(0.0f),
	rotateFac(1.0f),
	m_Flags(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground)
{
	m_Name = "MenuGenerateState";
}

MenuGenerateState::~MenuGenerateState()
{}

void MenuGenerateState::OnStart()
{
	printf("MenuGenerateState has been started!");
}

void MenuGenerateState::OnUpdate()
{
	glm::vec2 windowSize = RenderWindow::Instance()->GetWindowSize();

	//Galaxy generator text
	ImGui::SetNextWindowPos(ImVec2(windowSize.x * 0.5f - 50, windowSize.y * 0.5f - 92));
	ImGui::SetNextWindowSize(ImVec2(150, 15));
	ImGui::Begin("Generate Text", NULL, m_Flags);

	ImGui::Text("Galaxy Generator");

	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(windowSize.x * 0.5f - 175, windowSize.y * 0.5f - 72));
	ImGui::SetNextWindowSize(ImVec2(350, 112));
	ImGui::Begin("Generate", NULL, m_Flags);

	//Take input from the windows for generator
	ImGui::InputText("Seed", seed, 10, ImGuiInputTextFlags_CharsDecimal);
	ImGui::SliderInt("Number of Arms", &arms, 1, 7);
	ImGui::SliderFloat("ArmOffset", &armOffset, 0.0f, 1.0f);
	ImGui::SliderFloat("Rotation Factor", &rotateFac, 1.0f, 10.0f);
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(windowSize.x * 0.5f - 50, windowSize.y * 0.5f + 30));
	ImGui::SetNextWindowSize(ImVec2(200, 60));
	ImGui::Begin("Generate Button", NULL, m_Flags);

	//Generate button
	if (ImGui::Button("Generate", ImVec2(100, 30)))
	{
		printf("The generating has begun! This might take a while");
		m_Owner->ChangeState(m_Owner->GetState("MenuGameState"));
	}

	ImGui::End();
}

void MenuGenerateState::OnExit()
{
	Generator generator;

	generator.Init(
		PlanetManager::Instance()->GetPlanets(),
		seed,
		arms,
		armOffset,
		rotateFac
	);

	static_cast<Menu*>(m_Owner->GetOwner())->m_GameContext->InitGame();
}