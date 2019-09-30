//------------------------------------------------------------------------
// AIStateChasingFast
//
// Created:	2013/02/10
// Author:	Carel Boers
//	
// Fast chase behaviour state.
//------------------------------------------------------------------------

#ifndef STATECHASINGFAST_H
#define STATECHASINGFAST_H

#include "StateBase.h"
#include "GameObject.h"

namespace week9
{
	class AIStateChasingFast : public Common::StateBase
	{
	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		AIStateChasingFast();
		virtual ~AIStateChasingFast();

		// Overridden from StateBase
		virtual void Enter();
		virtual void Update(float p_fDelta);
		virtual void Exit();

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------

		// The GameObject we are chasing. Set when Enter() is called.
		Common::GameObject* m_pTargetGameObject;
	};
} // namespace week9

#endif // STATECHASINGFAST_H

