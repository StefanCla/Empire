#pragma once
#include "Headers/Precomp.h"
#include "Headers/FSM/FSM.h"

class FSM;

class FSMBase
{
public:
	FSMBase();
	~FSMBase();

	virtual void OnStart();
	virtual void OnUpdate() = 0;
	virtual void OnExit();

	const std::string& GetName() const { return m_Name; }
	void SetName(const std::string& name) { m_Name = name; }
	void SetOwner(FSM* owner) { m_Owner = owner; }

protected:
	std::string m_Name;
	FSM* m_Owner;
};