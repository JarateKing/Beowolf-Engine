//------------------------------------------------------------------------
// AIStateGrabbingCoin1
//
// Created:	2013/02/10
// Author:	Carel Boers
//	
// GrabbingCoin1 behaviour state.
//------------------------------------------------------------------------

#ifndef STATEGRABBINGCOIN1_H
#define STATEGRABBINGCOIN1_H

#include "StateBase.h"
#include "GameObject.h"
#include <list>

namespace week9
{
	class AIStateGrabbingCoin1 : public Common::StateBase
	{
	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		AIStateGrabbingCoin1();
		virtual ~AIStateGrabbingCoin1();

		// Overridden from StateBase
		virtual void Enter();
		virtual void Update(float p_fDelta);
		virtual void Exit();

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------

		// The GameObject we are GrabbingCoin1. Set when Enter() is called.
		Common::GameObject* m_pTargetGameObject;

		// Current path we are following
		std::list<glm::vec3> m_lPath;
	};
} // namespace week9

#endif // STATEGrabbingCoin1_H

