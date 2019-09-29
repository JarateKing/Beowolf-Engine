//------------------------------------------------------------------------
// ComponentCharacterController
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a character controller. It polls input and sends
// movement instructions to the relevant sibling components.
//------------------------------------------------------------------------

#include "W_Model.h"
#include "ComponentCoinTimer.h"
#include <cmath>
#include "ComponentRenderableMesh.h"
#include "ComponentBobbing.h"
#include "ComponentRotating.h"
#include "ComponentSphereCollider.h"
#include "ComponentLifespan.h"
#include "EventManager.h"
#include "W_RNG.h"

using namespace week2;

//------------------------------------------------------------------------------
// Method:    ComponentCharacterController
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
ComponentCoinTimer::ComponentCoinTimer(float spawnrate, int maxcount)
{
	m_spawnrate = spawnrate;
	m_maxcount = maxcount;

	m_time = 0;
	m_count = 0;

	EventManager::getInstance().AddListener("Destroyed", this);
}

//------------------------------------------------------------------------------
// Method:    ~ComponentCharacterController
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
ComponentCoinTimer::~ComponentCoinTimer()
{
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Polls input and sends movement instructions to relevant sibling components.
//------------------------------------------------------------------------------
void ComponentCoinTimer::Update(float p_fDelta)
{
	m_time += p_fDelta;

	if (m_time > m_spawnrate)
	{
		if (m_count < m_maxcount)
		{
			spawnCoin();
			m_count++;
		}
		m_time -= m_spawnrate;
	}
}

void ComponentCoinTimer::HandleEvent(void* details)
{
	EventDetails* dets = (EventDetails*)details;

	if (dets->id == "Destroyed")
	{
		m_count--;
	}
}

Common::GameObject* ComponentCoinTimer::spawnCoin()
{
	Common::GameObject* coin = GetGameObject()->GetManager()->CreateGameObject("../resources/objects/coin.xml");
	coin->GetTransform().Translate(glm::vec3(wolf::RNG::GetRandom(-25.0f, 25.0f), 5.0f, wolf::RNG::GetRandom(-25.0f, 25.0f)));
	coin->GetTransform().Scale(glm::vec3(0.5, 0.5, 0.5));

	return coin;
}

// FACTORY
Common::ComponentBase* ComponentCoinTimer::CreateComponent(TiXmlNode* p_node)
{
	float rate = 0.0f;
	int count = 0;

	for (TiXmlElement* i = p_node->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
	{
		std::string name = i->Value();

		if (name == "Spawnrate")
		{
			rate = std::atof(i->Attribute("value"));
		}
		else if (name == "MaxCount")
		{
			count = std::atoi(i->Attribute("value"));
		}
	}

	return new ComponentCoinTimer(rate, count);
}