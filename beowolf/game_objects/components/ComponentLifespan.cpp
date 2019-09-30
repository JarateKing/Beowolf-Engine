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
#include "ComponentLifespan.h"
#include "GameObject.h"
#include "EventManager.h"

using namespace week2;

//------------------------------------------------------------------------------
// Method:    ComponentCharacterController
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
ComponentLifespan::ComponentLifespan(double duration)
{
	m_time = duration;
}

//------------------------------------------------------------------------------
// Method:    ~ComponentCharacterController
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
ComponentLifespan::~ComponentLifespan()
{
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Polls input and sends movement instructions to relevant sibling components.
//------------------------------------------------------------------------------
void ComponentLifespan::Update(float p_fDelta)
{
	m_time -= p_fDelta;

	if (m_time <= 0)
	{
		GetGameObject()->ScheduleDelete();

		EventDetails* details = new EventDetails("Destroyed");
		EventManager::getInstance().QueueEvent(details);
	}
}

// FACTORY
Common::ComponentBase* ComponentLifespan::CreateComponent(TiXmlNode* p_node)
{
	float timer = 0.0f;

	for (TiXmlElement* i = p_node->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
	{
		std::string name = i->Value();

		if (name == "Timer")
		{
			timer = std::atof(i->Attribute("value"));
		}
	}

	return new ComponentLifespan(timer);
}