//------------------------------------------------------------------------
// AIStateGrabbingCoin2
//
// Created:	2013/02/10
// Author:	Carel Boers
//	
// GrabbingCoin2 behaviour state.
//------------------------------------------------------------------------

#ifndef STATEGRABBINGCOIN2_H
#define STATEGRABBINGCOIN2_H

#include "StateBase.h"
#include "GameObject.h"
#include <list>

namespace week9
{
	class AIStateGrabbingCoin2 : public Common::StateBase
	{
	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		AIStateGrabbingCoin2();
		virtual ~AIStateGrabbingCoin2();

		// Overridden from StateBase
		virtual void Enter();
		virtual void Update(float p_fDelta);
		virtual void Exit();

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------

		// The GameObject we are GrabbingCoin2. Set when Enter() is called.
		Common::GameObject* m_pTargetGameObject;

		// Current path we are following
		std::list<glm::vec3> m_lPath;
	};
} // namespace week9

#endif // STATEGrabbingCoin2_H

