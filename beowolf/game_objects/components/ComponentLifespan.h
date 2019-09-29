//------------------------------------------------------------------------
// ComponentCharacterController
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a character controller. It polls input and sends
// movement instructions to the relevant sibling components.
//------------------------------------------------------------------------

#ifndef COMPONENTLIFESPAN_H
#define COMPONENTLIFESPAN_H

#include "ComponentBase.h"
#include "tinyxml.h"

namespace week2
{
	class ComponentLifespan : public Common::ComponentBase
	{
	public:
		//------------------------------------------------------------------------------
		// Public types.
		//------------------------------------------------------------------------------

	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		ComponentLifespan(double duration);
		virtual ~ComponentLifespan();

		virtual const std::string FamilyID() { return std::string("GOC_Lifespan"); }
		virtual const std::string ComponentID(){ return std::string("GOC_Lifespan"); }
		virtual void Update(float p_fDelta);

		static ComponentBase* ComponentLifespan::CreateComponent(TiXmlNode* p_node);

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------
		double m_time;
	};
}

#endif // COMPONENTLOOKAT_H

