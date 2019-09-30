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
#include "ComponentStunOnCollision.h"
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
ComponentStunOnCollision::ComponentStunOnCollision()
{
	EventManager::getInstance().AddListener("Collision", this);
}

//------------------------------------------------------------------------------
// Method:    ~ComponentCharacterController
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
ComponentStunOnCollision::~ComponentStunOnCollision()
{
	EventManager::getInstance().RemoveListener("Collision", this);
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Polls input and sends movement instructions to relevant sibling components.
//------------------------------------------------------------------------------
void ComponentStunOnCollision::Update(float p_fDelta)
{
	m_stunTime -= p_fDelta;
}

void ComponentStunOnCollision::HandleEvent(void* details)
{
	EventDetails* dets = (EventDetails*)details;

	if (dets->id == "Collision")
	{
		if (dets->GetObj("first") == GetGameObject() && dets->GetObj("second")->GetGUID() == "Ball")
		{
			m_stunTime = 2.0f;
		}
	}
}

// FACTORY
Common::ComponentBase* ComponentStunOnCollision::CreateComponent(TiXmlNode* p_node)
{
	return new ComponentStunOnCollision();
}

bool ComponentStunOnCollision::isStunned()
{
	return m_stunTime > 0.0f;
}