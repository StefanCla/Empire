#pragma once
#include "Headers/Precomp.h"
#include "Headers/GameContext.h"

//MenuState
#include "Headers/FSM/FSM.h"
#include "Headers/FSM/FSMObjectBase.h"
#include "Headers/MenuStates/MenuStartState.h"
#include "Headers/MenuStates/MenuGenerateState.h"
#include "Headers/MenuStates/MenuGameState.h"

//Dear ImGui
#include "ImGui/imgui.h"

#ifdef LINUX
#include "../Empire_Linux/ImGui/imgui_impl_opengl3.h"
#elif WINDOWS
#include "../Empire_Windows/ImGui/imgui_impl_glfw.h"
#include "../Empire_Windows/ImGui/imgui_impl_opengl3.h"
#endif


class MenuGameState;
class MenuGenerateState;
class GameContext;

class Menu : public FSMObjectBase
{
public:
	Menu(const char* version, GameContext* gameContext);
	~Menu();

	void Render();

	GameContext* m_GameContext;

private:
	ImGuiIO* m_IO;
	FSM* m_MenuState;
	MenuStartState* m_StartState;
	MenuGenerateState* m_GenerateState;
	MenuGameState* m_GameState;

};