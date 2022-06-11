#include "Headers/Menu.h"

Menu::Menu(const char* version, GameContext* gameContext)
{
	m_GameContext = gameContext;
	m_StartState = new MenuStartState();
	m_GenerateState = new MenuGenerateState();
	m_GameState = new MenuGameState();
	m_MenuState = new FSM(this);

	m_MenuState->RegisterState(m_StartState);
	m_MenuState->RegisterState(m_GenerateState);
	m_MenuState->RegisterState(m_GameState);

	m_MenuState->FirstState(m_StartState);

	//Create & init ImGui
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	io.DisplaySize.x = SCREENWIDTH;
	io.DisplaySize.y = SCREENHEIGHT;

	//Load DroidSans font
#ifdef WINDOWS
	io.Fonts->AddFontFromFileTTF("../Empire/Resources/Fonts/DroidSans.ttf", 15);
#elif LINUX
	io.Fonts->AddFontFromFileTTF("../../../Empire/Resources/Fonts/DroidSans.ttf", 15);
#endif

	m_IO = &io;

	ImGui::StyleColorsDark();

	ImGui_ImplOpenGL3_Init(version);

#ifdef WINDOWS
	ImGui_ImplGlfw_InitForOpenGL(RenderWindow::Instance()->m_Window, true);
#endif //endfi WINDOWS
	
}

Menu::~Menu()
{
	//Delete menu states
	delete m_MenuState;
	delete m_GameState;
	delete m_GenerateState;
	delete m_StartState;
}

void Menu::Render()
{
	//Start
	ImGui_ImplOpenGL3_NewFrame();
#ifdef WINDOWS
	ImGui_ImplGlfw_NewFrame();
#endif //WINDOWS

	ImGui::NewFrame();

	//Update menu
	m_MenuState->Update();

	//End
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}