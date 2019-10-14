//------------------------------------------------------------------------
// ComponentCharacterController
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a character controller. It polls input and sends
// movement instructions to the relevant sibling components.
//------------------------------------------------------------------------

#ifndef COMPNENTCHARACTERCONTROLLER_H
#define COMPNENTCHARACTERCONTROLLER_H

#include "ComponentBase.h"
#include "tinyxml.h"
#include "ComponentRigidBody.h"
#include "JSON/json.hpp"

using json = nlohmann::json;

namespace week2
{
	class ComponentCharacterController : public Common::ComponentBase
	{
	public:
		//------------------------------------------------------------------------------
		// Public types.
		//------------------------------------------------------------------------------

	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		ComponentCharacterController();
		virtual ~ComponentCharacterController();

		virtual const std::string FamilyID() { return std::string("GOC_CharacterController"); }
		virtual const std::string ComponentID(){ return std::string("GOC_CharacterController"); }
		virtual void Update(float p_fDelta);

		static ComponentBase* ComponentCharacterController::CreateComponent(json p_pNode);

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------

		// Keys buffer
		bool m_bKeysDown[256];
		bool m_bKeysDownLast[256];

		glm::quat m_rot;
		Common::ComponentRigidBody* m_rigidbody = NULL;
	};
}

#endif // COMPNENTCHARACTERCONTROLLER_H

