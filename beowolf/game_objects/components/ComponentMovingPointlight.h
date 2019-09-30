//------------------------------------------------------------------------
// ComponentCharacterController
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a character controller. It polls input and sends
// movement instructions to the relevant sibling components.
//------------------------------------------------------------------------

#ifndef COMPONENTMOVINGPOINTLIGHT_H
#define COMPONENTMOVINGPOINTLIGHT_H

#include "ComponentBase.h"
#include "SceneManager.h"

namespace week2
{
	class ComponentMovingPointlight : public Common::ComponentBase
	{
	public:
		//------------------------------------------------------------------------------
		// Public types.
		//------------------------------------------------------------------------------

	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		ComponentMovingPointlight();
		virtual ~ComponentMovingPointlight();

		virtual const std::string FamilyID() { return std::string("GOC_MovingPointlight"); }
		virtual const std::string ComponentID(){ return std::string("GOC_MovingPointlight"); }
		virtual void Update(float p_fDelta);

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------
	};
}

#endif // COMPONENTLOOKAT_H

