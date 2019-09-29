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
#include "ComponentBobbing.h"
#include "ComponentBase.h"
#include "GameObject.h"
#include <cmath>
#include <stdlib.h>

using namespace week2;

//------------------------------------------------------------------------------
// Method:    ComponentCharacterController
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
ComponentBobbing::ComponentBobbing(double rate, double scale)
{
	m_time = 0;
	m_oldval = 0;
	m_rate = rate;
	m_scale = scale;
}

//------------------------------------------------------------------------------
// Method:    ~ComponentCharacterController
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
ComponentBobbing::~ComponentBobbing()
{
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Polls input and sends movement instructions to relevant sibling components.
//------------------------------------------------------------------------------
void ComponentBobbing::Update(float p_fDelta)
{
	m_time += p_fDelta;

	double newval = sin(m_time * m_rate) * m_scale;
	GetGameObject()->GetTransform().SetTranslation(GetGameObject()->GetTransform().GetTranslation() - glm::vec3(0, m_oldval, 0) + glm::vec3(0, newval, 0));

	m_oldval = newval;
}

// FACTORY
Common::ComponentBase* ComponentBobbing::CreateComponent(TiXmlNode* p_node)
{
	float rate = 0.0f;
	float scale = 0.0f;

	for (TiXmlElement* i = p_node->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
	{
		std::string name = i->Value();

		if (name == "Rate")
		{
			rate = std::atof(i->Attribute("value"));
		}
		else if (name == "Scale")
		{
			scale = std::atof(i->Attribute("value"));
		}
	}

	return new ComponentBobbing(rate, scale);
}