//------------------------------------------------------------------------
// ComponentCharacterController
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a character controller. It polls input and sends
// movement instructions to the relevant sibling components.
//------------------------------------------------------------------------

#ifndef COMPONENTSCORE_H
#define COMPONENTSCORE_H

#include "ComponentBase.h"
#include "tinyxml.h"
#include "JSON/json.hpp"

using json = nlohmann::json;

namespace week2
{
	class ComponentScore : public Common::ComponentBase
	{
	public:
		//------------------------------------------------------------------------------
		// Public types.
		//------------------------------------------------------------------------------

	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		ComponentScore(int value);
		virtual ~ComponentScore();

		virtual const std::string FamilyID() { return std::string("GOC_Score"); }
		virtual const std::string ComponentID(){ return std::string("GOC_Score"); }
		virtual void Update(float p_fDelta);

		static ComponentBase* ComponentScore::CreateComponent(json p_node);

		void HandleEvent(void* details);

		int GetScore();

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------
		int m_score;
	};
}

#endif // COMPONENTLOOKAT_H

