//------------------------------------------------------------------------
// StateBase
//
// Created:	2013/02/10
// Author:	Carel Boers
//	
// Base class for any states driven by StateMachine.
//------------------------------------------------------------------------

#ifndef STATEBASE_H
#define STATEBASE_H

#include "StateMachine.h"

namespace Common
{
	class StateBase
	{
		// To allow setting membership.
		friend class StateMachine;

	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		StateBase() : m_pStateMachine(NULL) {}
		virtual ~StateBase() {}
			
		// Required interface for all subclasses.
		virtual void Enter() = 0;
		virtual void Update(float p_fDelta) = 0;
		virtual void Exit() = 0;

	private:
		//------------------------------------------------------------------------------
		// Private methods.
		//------------------------------------------------------------------------------

		void SetStateMachineMembership(StateMachine* p_pStateMachine) { m_pStateMachine = p_pStateMachine; }

	protected:
		//------------------------------------------------------------------------------
		// Protected members
		//------------------------------------------------------------------------------
		
		// State machine that this state is registered with
		StateMachine* m_pStateMachine;
	};
} // namespace Common

#endif

