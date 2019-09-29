//------------------------------------------------------------------------
// ComponentCharacterController
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a character controller. It polls input and sends
// movement instructions to the relevant sibling components.
//------------------------------------------------------------------------

#ifndef COMPONENTCOINTIMER_H
#define COMPONENTCOINTIMER_H

#include "ComponentBase.h"
#include "GameObject.h"
#include "tinyxml.h"

namespace week2
{
	class ComponentCoinTimer : public Common::ComponentBase
	{
	public:
		//------------------------------------------------------------------------------
		// Public types.
		//------------------------------------------------------------------------------

	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		ComponentCoinTimer(float spawnrate, int maxcount);
		virtual ~ComponentCoinTimer();

		virtual const std::string FamilyID() { return std::string("GOC_CoinTimer"); }
		virtual const std::string ComponentID(){ return std::string("GOC_CoinTimer"); }
		virtual void Update(float p_fDelta);

		static ComponentBase* ComponentCoinTimer::CreateComponent(TiXmlNode* p_node);

		void HandleEvent(void* details);

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------
		Common::GameObject* spawnCoin();

		float m_spawnrate;
		int m_maxcount;
		double m_time;
		int m_count;
	};
}

#endif // COMPONENTLOOKAT_H

