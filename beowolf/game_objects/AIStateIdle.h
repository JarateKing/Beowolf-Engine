//------------------------------------------------------------------------
// AIStateIdle
//
// Created:	2013/02/10
// Author:	Carel Boers
//	
// Idle behaviour state.
//------------------------------------------------------------------------

#ifndef STATEIDLE_H
#define STATEIDLE_H

#include "StateBase.h"

namespace week9
{
	class AIStateIdle : public Common::StateBase
	{
	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		AIStateIdle();
		virtual ~AIStateIdle();

		// Overridden from StateBase
		virtual void Enter();
		virtual void Update(float p_fDelta);
		virtual void Exit();
	};
} // namespace week9

#endif // STATEIDLE_H

