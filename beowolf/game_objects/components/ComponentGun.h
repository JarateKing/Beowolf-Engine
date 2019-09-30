//------------------------------------------------------------------------
// ComponentCharacterController
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a character controller. It polls input and sends
// movement instructions to the relevant sibling components.
//------------------------------------------------------------------------

#ifndef COMPONENTGUN_H
#define COMPONENTGUN_H

#include "ComponentBase.h"
#include "GameObject.h"
#include "tinyxml.h"

namespace week2
{
	class ComponentGun : public Common::ComponentBase
	{
	public:
		//------------------------------------------------------------------------------
		// Public types.
		//------------------------------------------------------------------------------

	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		ComponentGun(float firerate);
		virtual ~ComponentGun();

		virtual const std::string FamilyID() { return std::string("GOC_Gun"); }
		virtual const std::string ComponentID(){ return std::string("GOC_Gun"); }
		virtual void Update(float p_fDelta);

		static ComponentBase* ComponentGun::CreateComponent(TiXmlNode* p_node);

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------
		Common::GameObject* spawnBall();

		float m_firerate;
		float m_time;
	};
}

#endif // COMPONENTLOOKAT_H

