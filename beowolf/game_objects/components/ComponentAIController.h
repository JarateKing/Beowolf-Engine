//------------------------------------------------------------------------
// ComponentAIController
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements an AI controller. It loads a decision tree from
// XML and processes the game state to decide it's best state.
//------------------------------------------------------------------------

#ifndef COMPNENTAICONTROLLER_H
#define COMPNENTAICONTROLLER_H

#include "ComponentBase.h"
#include "AIDecisionTree.h"
#include "StateMachine.h"

namespace week9
{
	class ComponentAIController : public Common::ComponentBase
	{
	public:
		//------------------------------------------------------------------------------
		// Public types.
		//------------------------------------------------------------------------------

		// Supported AI behaviours
		enum AIState
		{
			eAIState_None = -1,
			eAIState_Idle = 0,
			eAIState_Wander,
			eAIState_Chasing,
			eAIState_GrabbingCoin1,
			eAIState_GrabbingCoin2
		};

	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		ComponentAIController();
		virtual ~ComponentAIController();

		virtual const std::string FamilyID() { return std::string("GOC_AIController"); }
		virtual const std::string ComponentID(){ return std::string("GOC_AIController"); }
		virtual void Update(float p_fDelta);

		virtual void Init(const char* p_strDecisionTreePath);

		Common::StateMachine* GetStateMachine() { return m_pStateMachine; }

		// Mapping method
		AIState MapActionToState(const std::string& p_strAction)
		{
			if (p_strAction.compare("ACTION_CHASE") == 0)			{ return eAIState_Chasing; }
			else if (p_strAction.compare("ACTION_GRABBINGCOIN1") == 0)	{ return eAIState_GrabbingCoin1; }
			else if (p_strAction.compare("ACTION_GRABBINGCOIN2") == 0)	{ return eAIState_GrabbingCoin2; }
			else if (p_strAction.compare("ACTION_IDLE") == 0)		{ return eAIState_Idle; }
			else if (p_strAction.compare("ACTION_WANDER") == 0)		{ return eAIState_Wander; }	
			else													{ return eAIState_None; }
		}

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------

		// Our decision tree
		AIDecisionTree* m_pDecisionTree;

		// Our state machine
		Common::StateMachine* m_pStateMachine;
	};
}

#endif // COMPNENTAICONTROLLER_H

