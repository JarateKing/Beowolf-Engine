//------------------------------------------------------------------------
// ComponentCharacterController
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a character controller. It polls input and sends
// movement instructions to the relevant sibling components.
//------------------------------------------------------------------------

#ifndef COMPONENTBOBBING_H
#define COMPONENTBOBBING_H

#include "ComponentBase.h"
#include "tinyxml.h"

namespace week2
{
	class ComponentBobbing : public Common::ComponentBase
	{
	public:
		//------------------------------------------------------------------------------
		// Public types.
		//------------------------------------------------------------------------------

	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		ComponentBobbing(double rate, double scale);
		virtual ~ComponentBobbing();

		virtual const std::string FamilyID() { return std::string("GOC_Bobbing"); }
		virtual const std::string ComponentID(){ return std::string("GOC_Bobbing"); }
		virtual void Update(float p_fDelta);
		
		static ComponentBase* CreateComponent(TiXmlNode* p_node);

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------
		double m_time;
		double m_rate;
		double m_scale;
		double m_oldval;
	};
}

#endif // COMPONENTLOOKAT_H

