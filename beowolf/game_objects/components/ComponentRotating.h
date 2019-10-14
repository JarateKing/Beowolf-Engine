//------------------------------------------------------------------------
// ComponentCharacterController
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a character controller. It polls input and sends
// movement instructions to the relevant sibling components.
//------------------------------------------------------------------------

#ifndef COMPONENTROTATING_H
#define COMPONENTROTATING_H

#include "ComponentBase.h"
#include "tinyxml.h"
#include "JSON/json.hpp"

using json = nlohmann::json;

namespace week2
{
	class ComponentRotating : public Common::ComponentBase
	{
	public:
		//------------------------------------------------------------------------------
		// Public types.
		//------------------------------------------------------------------------------

	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		ComponentRotating(double rate);
		virtual ~ComponentRotating();

		virtual const std::string FamilyID() { return std::string("GOC_Rotating"); }
		virtual const std::string ComponentID(){ return std::string("GOC_Rotating"); }
		virtual void Update(float p_fDelta);

		static ComponentBase* ComponentRotating::CreateComponent(json p_node);

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------
		double m_rate;
	};
}

#endif // COMPONENTLOOKAT_H

