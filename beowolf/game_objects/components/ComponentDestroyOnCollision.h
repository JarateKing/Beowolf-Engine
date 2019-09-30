//------------------------------------------------------------------------
// ComponentCharacterController
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a character controller. It polls input and sends
// movement instructions to the relevant sibling components.
//------------------------------------------------------------------------

#ifndef COMPONENTDESTROYONCOLLISION_H
#define COMPONENTDESTROYONCOLLISION_H

#include "ComponentBase.h"
#include "GameObject.h"
#include "tinyxml.h"

namespace week2
{
	class ComponentDestroyOnCollision : public Common::ComponentBase
	{
	public:
		//------------------------------------------------------------------------------
		// Public types.
		//------------------------------------------------------------------------------

	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		ComponentDestroyOnCollision();
		virtual ~ComponentDestroyOnCollision();

		virtual const std::string FamilyID() { return std::string("GOC_DestroyOnCollision"); }
		virtual const std::string ComponentID(){ return std::string("GOC_DestroyOnCollision"); }
		virtual void Update(float p_fDelta);

		static ComponentBase* ComponentDestroyOnCollision::CreateComponent(TiXmlNode* p_node);

		void HandleEvent(void* details);

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------
	};
}

#endif // COMPONENTLOOKAT_H

