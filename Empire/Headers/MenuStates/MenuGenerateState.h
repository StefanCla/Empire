#pragma once
#include "Headers/Precomp.h"
#include "Headers/Menu.h"
#include "Headers/FSM/FSMBase.h"
#include "Headers/Generator.h"

#include "ImGui/imconfig.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imstb_rectpack.h"
#include "ImGui/imstb_textedit.h"
#include "ImGui/imstb_truetype.h"

class MenuGenerateState : public FSMBase
{
public:
	MenuGenerateState();
	virtual ~MenuGenerateState();
	void OnStart();
	void OnUpdate();
	void OnExit();

private:
	char seed[10] = "";
	int arms;
	float armOffset;
	float rotateFac;
	ImGuiWindowFlags m_Flags;

};