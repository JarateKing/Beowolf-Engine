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
#include "ComponentRotating.h"
#include "GameObject.h"
#include <cmath>

using namespace week2;

//------------------------------------------------------------------------------
// Method:    ComponentCharacterController
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
ComponentRotating::ComponentRotating(double rate)
{
	m_rate = rate;
}

//------------------------------------------------------------------------------
// Method:    ~ComponentCharacterController
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
ComponentRotating::~ComponentRotating()
{
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Polls input and sends movement instructions to relevant sibling components.
//------------------------------------------------------------------------------
void ComponentRotating::Update(float p_fDelta)
{
	GetGameObject()->GetTransform().RotateXYZ(0, m_rate * p_fDelta, 0);
}

// FACTORY
Common::ComponentBase* ComponentRotating::CreateComponent(json p_node)
{
	float rate = 0.0f;

	size_t it_h = 0;
	auto& gameObjects = p_node["GameObject"];

	for (auto& gameObject : gameObjects)
	{
		if (gameObject[it_h]["Component Name"] == "GOC_Rotating")
		{
			std::string rates = gameObject[it_h]["Rate value"];
			rate = std::stof(rates);
		}
	}

	/*for (TiXmlElement* i = p_node->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
	{
		std::string name = i->Value();

		if (name == "Rate")
		{
			rate = std::atof(i->Attribute("value"));
		}
	}*/

	return new ComponentRotating(rate);
}