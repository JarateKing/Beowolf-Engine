//------------------------------------------------------------------------
// ComponentCharacterController
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a character controller. It polls input and sends
// movement instructions to the relevant sibling components.
//------------------------------------------------------------------------

#ifndef COMPONENTITEMSTATS_H
#define COMPONENTITEMSTATS_H

#include "ComponentBase.h"
#include "tinyxml.h"
#include "JSON/json.hpp"
#include <vector>
#include <map>
#include <algorithm>

using json = nlohmann::json;

namespace week2
{
	class ComponentItemStats : public Common::ComponentBase
	{
	public:
		//------------------------------------------------------------------------------
		// Public types.
		//------------------------------------------------------------------------------

	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		ComponentItemStats(std::vector<std::pair<std::string, float>> values);
		virtual ~ComponentItemStats();

		virtual const std::string FamilyID() { return std::string("GOC_ItemStats"); }
		virtual const std::string ComponentID(){ return std::string("GOC_ItemStats"); }
		virtual void Update(float p_fDelta);

		static ComponentBase* ComponentItemStats::CreateComponent(json p_node);

		float GetValue(std::string id);

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------
		std::map<std::string, float> m_statMult;
	};
}

#endif // COMPONENTLOOKAT_H

