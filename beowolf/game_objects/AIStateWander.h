//------------------------------------------------------------------------
// AIStateWander
//
// Created:	2013/02/10
// Author:	Carel Boers
//	
// Wander behaviour state.
//------------------------------------------------------------------------

#ifndef STATEWANDER_H
#define STATEWANDER_H

#include "StateBase.h"
#include "Transform.h"
#include "ComponentAIController.h"

namespace week9
{
	class AIStateWander : public Common::StateBase
	{
	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		AIStateWander();
		virtual ~AIStateWander();

		// Overridden from StateBase
		virtual void Enter();
		virtual void Update(float p_fDelta);
		virtual void Exit();

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------

		// Our wander destination. Set when Enter() is called.
		glm::vec3 ChooseDestination(ComponentAIController* controller);
		glm::vec3 m_vDestination;
	};
} // namespace week9

#endif // STATEWANDER_H

