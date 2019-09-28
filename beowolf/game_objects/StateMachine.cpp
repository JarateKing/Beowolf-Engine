//------------------------------------------------------------------------
// StateMachine
//
// Created:	2013/02/10
// Author:	Carel Boers
//	
// State machine that manages state transitions.
//------------------------------------------------------------------------

#include "StateBase.h"
#include "StateMachine.h"
#include "assert.h"

using namespace Common;

//------------------------------------------------------------------------------
// Method:    StateMachine
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
StateMachine::StateMachine()
	:
	m_iCurrentState(-1),
	m_pCurrentState(NULL),
	m_fCurrentStateTime(0.0f),
	m_pOwner(NULL)
{
}

//------------------------------------------------------------------------------
// Method:    ~StateMachine
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
StateMachine::~StateMachine()
{
}

//------------------------------------------------------------------------------
// Method:    RegisterState
// Parameter: int p_iState
// Parameter: StateBase * p_pstate
// Returns:   void
// 
// Register a given state with the state manager
//------------------------------------------------------------------------------
void StateMachine::RegisterState(int p_iState, StateBase* p_pstate)
{
	p_pstate->SetStateMachineMembership(this);
	m_mStateMap.insert(std::pair<int, StateBase*>(p_iState, p_pstate));
}

//------------------------------------------------------------------------------
// Method:    GoToState
// Parameter: int p_iState
// Returns:   void
// 
// Go to the given state. Make it the active state.
//------------------------------------------------------------------------------
void StateMachine::GoToState(int p_iState)
{
	// Prevent double entering current state
	if (p_iState == m_iCurrentState)
	{
		return;
	}

	StateBase* pState = NULL;
	StateMap::iterator it = m_mStateMap.find(p_iState);
	if (it != m_mStateMap.end())
	{
		pState = static_cast<StateBase*>(it->second);
	}
	assert(pState);

	// Call exit on current state
	if (m_pCurrentState)
	{
		m_pCurrentState->Exit();
	}

	// Call enter on the new state
	pState->Enter();

	// Set the new current state.
	m_iCurrentState = p_iState;
	m_pCurrentState = pState;
	m_fCurrentStateTime = 0.0f;
}

//------------------------------------------------------------------------------
// Method:    PushState
// Parameter: int p_iState
// Returns:   void
// 
// Push the given state onto our state stack.
//------------------------------------------------------------------------------
void StateMachine::PushState(int p_iState)
{
	assert(false);	// NOT YET IMPLEMENTED
}

//------------------------------------------------------------------------------
// Method:    PopState
// Returns:   void
// 
// Pop the top state from our state stack.
//------------------------------------------------------------------------------
void StateMachine::PopState()
{
	assert(false);	// NOT YET IMPLEMENTED
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Updates the currently active state
//------------------------------------------------------------------------------
void StateMachine::Update(float p_fDelta)
{
	m_fCurrentStateTime += p_fDelta;
	if (m_pCurrentState)
	{
		m_pCurrentState->Update(p_fDelta);
	}
}