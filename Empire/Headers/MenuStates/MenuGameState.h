#pragma once
#include "Headers/Precomp.h"
#include "Headers/Menu.h"
#include "Headers/FSM/FSMBase.h"

#include "ImGui/imconfig.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imstb_rectpack.h"
#include "ImGui/imstb_textedit.h"
#include "ImGui/imstb_truetype.h"

class MenuGameState : public FSMBase
{
public:
	MenuGameState();
	virtual ~MenuGameState();
	void OnStart();
	void OnUpdate();
	void DisplayData(GameObject* object);

private:
	int m_Selected;
	char m_Buffer[15];
	bool m_CVBuyMenuSwitch;

	ImGuiWindowFlags m_Flags;
};
