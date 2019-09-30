//------------------------------------------------------------------------
// AIStateChasing
//
// Created:	2013/02/10
// Author:	Carel Boers
//	
// Chasing behaviour state.
//------------------------------------------------------------------------

#ifndef STATECHASING_H
#define STATECHASING_H

#include "StateBase.h"
#include "GameObject.h"
#include <list>

namespace week9
{
	class AIStateChasing : public Common::StateBase
	{
	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		AIStateChasing();
		virtual ~AIStateChasing();

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

		// Current path we are following
		std::list<glm::vec3> m_lPath;
	};
} // namespace week9

#endif // STATECHASING_H

