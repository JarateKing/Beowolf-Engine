//------------------------------------------------------------------------
// ComponentCharacterController
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a character controller. It polls input and sends
// movement instructions to the relevant sibling components.
//------------------------------------------------------------------------

#ifndef COMPONENTSTUNONCOLLISION_H
#define COMPONENTSTUNONCOLLISION_H

#include "ComponentBase.h"
#include "GameObject.h"
#include "tinyxml.h"

namespace week2
{
	class ComponentStunOnCollision : public Common::ComponentBase
	{
	public:
		//------------------------------------------------------------------------------
		// Public types.
		//------------------------------------------------------------------------------

	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		ComponentStunOnCollision();
		virtual ~ComponentStunOnCollision();

		virtual const std::string FamilyID() { return std::string("GOC_StunOnCollision"); }
		virtual const std::string ComponentID(){ return std::string("GOC_StunOnCollision"); }
		virtual void Update(float p_fDelta);

		static ComponentBase* ComponentStunOnCollision::CreateComponent(TiXmlNode* p_node);

		void HandleEvent(void* details);

		bool isStunned();

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------
		float m_stunTime;
	};
}

#endif // COMPONENTLOOKAT_H

