#pragma once
#include "Headers/Precomp.h"
#include "Headers/FSM/FSMBase.h"

#include "ImGui/imconfig.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imstb_rectpack.h"
#include "ImGui/imstb_textedit.h"
#include "ImGui/imstb_truetype.h"

class MenuStartState : public FSMBase
{
public:
	MenuStartState();
	virtual ~MenuStartState();
	void OnStart();
	void OnUpdate();

private:
	ImGuiWindowFlags m_Flags;
};