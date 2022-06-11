#include "Headers/FSM/FSMBase.h"

FSMBase::FSMBase() :
	m_Name(""),
	m_Owner(nullptr)
{}

FSMBase::~FSMBase()
{}

void FSMBase::OnStart()
{
	//printf("The Start state has been called\n");
}

void FSMBase::OnExit()
{
	//printf("The Exit state has been called\n");
}