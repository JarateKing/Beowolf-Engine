//------------------------------------------------------------------------
// ComponentCharacterController
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a character controller. It polls input and sends
// movement instructions to the relevant sibling components.
//------------------------------------------------------------------------

#ifndef COMPONENTFOLLOW_H
#define COMPONENTFOLLOW_H

#include "ComponentBase.h"

namespace week2
{
	class ComponentFollow : public Common::ComponentBase
	{
	public:
		//------------------------------------------------------------------------------
		// Public types.
		//------------------------------------------------------------------------------

	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		ComponentFollow();
		virtual ~ComponentFollow();

		virtual const std::string FamilyID() { return std::string("GOC_Follow"); }
		virtual const std::string ComponentID(){ return std::string("GOC_Follow"); }
		virtual void Follow(Common::GameObject* p_target);
		virtual void Update(float p_fDelta);
		virtual void SetOffset(glm::vec3 p_offset);

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------
		Common::GameObject* m_target;
		glm::vec3 m_offset;
	};
}

#endif // COMPONENTLOOKAT_H

