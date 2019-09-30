//------------------------------------------------------------------------
// StateMachine
//
// Created:	2013/02/10
// Author:	Carel Boers
//	
// State machine that manages state transitions.
//------------------------------------------------------------------------

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <map>

namespace Common
{
	// Forward declaration
	class StateBase;

	class StateMachine
	{
	private:
		//---------------------------------------------------------------------
		// Private types
		//---------------------------------------------------------------------

		typedef std::map<int, StateBase*> StateMap;

	public:
		//---------------------------------------------------------------------
		// Public methods.
		//---------------------------------------------------------------------
		StateMachine(void);
		~StateMachine(void);

		void RegisterState(int p_iState, StateBase* p_pstate);
		void GoToState(int p_iState);
		void PushState(int p_iState);
		void PopState();

		// Update current state
		void Update(float p_fDelta);

		int GetCurrentState()		{ return m_iCurrentState; }
		float GetCurrentStateTime() { return m_fCurrentStateTime; }

		// A way for states in a state machine to reference back to their owner, whether 
		// it's the Game class, a Character controller or an AI controller.
		void SetStateMachineOwner(void *p_pOwner)	{ m_pOwner = p_pOwner; }
		void* GetStateMachineOwner()				{ return m_pOwner; }

	private:
		//---------------------------------------------------------------------
		// Private members.
		//---------------------------------------------------------------------

		// Map of state Ids to state instances
		StateMap m_mStateMap;

		// Current state
		int m_iCurrentState;
		StateBase* m_pCurrentState;

		// State timer
		float m_fCurrentStateTime;

		// State machine owner
		void* m_pOwner;
	};
} // namespace Common

#endif // STATEMACHINE_H

