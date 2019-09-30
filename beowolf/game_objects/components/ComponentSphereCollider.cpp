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
#include "ComponentSphereCollider.h"
#include "GameObject.h"
#include <cmath>
#include "EventManager.h"

using namespace week2;

//------------------------------------------------------------------------------
// Method:    ComponentCharacterController
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
ComponentSphereCollider::ComponentSphereCollider(float radius)
{
	m_radius = radius;
}

//------------------------------------------------------------------------------
// Method:    ~ComponentCharacterController
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
ComponentSphereCollider::~ComponentSphereCollider()
{
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Polls input and sends movement instructions to relevant sibling components.
//------------------------------------------------------------------------------
void ComponentSphereCollider::Update(float p_fDelta)
{
	auto it = GetGameObject()->GetManager()->Begin();
	for (; it != GetGameObject()->GetManager()->End(); ++it)
	{
		ComponentBase* current = it->second->GetComponent("GOC_SphereCollider");
		if (current != NULL && current != this)
		{
			ComponentSphereCollider* curCollider = (ComponentSphereCollider*)current;

			float dist1 = glm::distance(GetGameObject()->GetTransform().GetTranslation(), it->second->GetTransform().GetTranslation());
			float dist2 = m_radius + curCollider->GetRadius();

			if (dist1 < dist2)
			{
				EventDetails* details = new EventDetails("Collision");
				details->AddObj("first", GetGameObject());
				details->AddObj("second", it->second);
				EventManager::getInstance().QueueEvent(details);
			}
		}
	}
}

float ComponentSphereCollider::GetRadius()
{
	return m_radius;
}

// FACTORY
Common::ComponentBase* ComponentSphereCollider::CreateComponent(TiXmlNode* p_node)
{
	float radius = 0.0f;

	for (TiXmlElement* i = p_node->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
	{
		std::string name = i->Value();

		if (name == "Radius")
		{
			radius = std::atof(i->Attribute("value"));
		}
	}

	return new ComponentSphereCollider(radius);
}