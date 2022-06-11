#include "Headers/FSM/FSM.h"

FSM::FSM(FSMObjectBase* owner)
{
	m_BaseOwner = owner;
}

FSM::~FSM()
{
}

void FSM::RegisterState(FSMBase* state)
{
	m_AllStates.insert(std::make_pair(state->GetName(), state));
	state->SetOwner(this);
}

void FSM::FirstState(FSMBase* state)
{
	m_CurrentState = state;
	m_PreviousState = m_CurrentState;
	m_CurrentState->OnStart();
}

void FSM::ChangeState(FSMBase* state)
{
	m_PreviousState = m_CurrentState;
	m_CurrentState = state;
}

void FSM::Update()
{
	m_CurrentState->OnUpdate();

	if (m_CurrentState != m_PreviousState)
	{
		m_PreviousState->OnExit();
		m_CurrentState->OnStart();
		m_PreviousState = m_CurrentState;
	}
}

FSMBase* FSM::GetState(const std::string& name) const
{
	std::map<std::string, FSMBase*>::const_iterator it = m_AllStates.find(name);

	if (it != m_AllStates.end())
		return it->second;
	else
		return nullptr;
}