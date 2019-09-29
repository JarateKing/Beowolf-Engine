//------------------------------------------------------------------------
// ComponentCharacterController
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a character controller. It polls input and sends
// movement instructions to the relevant sibling components.
//------------------------------------------------------------------------

#ifndef COMPONENTSPHERECOLLIDER_H
#define COMPONENTSPHERECOLLIDER_H

#include "ComponentBase.h"
#include "tinyxml.h"

namespace week2
{
	class ComponentSphereCollider : public Common::ComponentBase
	{
	public:
		//------------------------------------------------------------------------------
		// Public types.
		//------------------------------------------------------------------------------

	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		ComponentSphereCollider(float radius);
		virtual ~ComponentSphereCollider();

		virtual const std::string FamilyID() { return std::string("GOC_SphereCollider"); }
		virtual const std::string ComponentID(){ return std::string("GOC_SphereCollider"); }
		virtual void Update(float p_fDelta);

		static ComponentBase* CreateComponent(TiXmlNode* p_node);

		float GetRadius();

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------
		float m_radius;
	};
}

#endif // COMPONENTLOOKAT_H

