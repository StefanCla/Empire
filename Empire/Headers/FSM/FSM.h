#pragma once
#include "Headers/Precomp.h"
#include "Headers/FSM/FSMBase.h"
#include <map>

class FSMBase;
class FSMObjectBase;

// FSM based on code from Abhishek Biswas
class FSM
{
public:
	FSM(FSMObjectBase* owner);
	~FSM();

	void RegisterState(FSMBase* state);
	void FirstState(FSMBase* state);
	void ChangeState(FSMBase* state);
	void Update();

	FSMBase* GetState(const std::string& name) const;
	FSMObjectBase* GetOwner() const { return m_BaseOwner; }

private:
	std::map<std::string, FSMBase*> m_AllStates;

	FSMBase* m_CurrentState;
	FSMBase* m_PreviousState;
	FSMObjectBase* m_BaseOwner;
};