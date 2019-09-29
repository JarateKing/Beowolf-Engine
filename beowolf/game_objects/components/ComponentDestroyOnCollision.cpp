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
#include "ComponentDestroyOnCollision.h"
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
ComponentDestroyOnCollision::ComponentDestroyOnCollision()
{
	EventManager::getInstance().AddListener("Collision", this);
}

//------------------------------------------------------------------------------
// Method:    ~ComponentCharacterController
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
ComponentDestroyOnCollision::~ComponentDestroyOnCollision()
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
void ComponentDestroyOnCollision::Update(float p_fDelta)
{
}

void ComponentDestroyOnCollision::HandleEvent(void* details)
{
	EventDetails* dets = (EventDetails*)details;

	if (dets->id == "Collision")
	{
		if (dets->GetObj("first") == GetGameObject())
		{
			GetGameObject()->ScheduleDelete();

			EventDetails* details = new EventDetails("Destroyed");
			EventManager::getInstance().QueueEvent(details);
		}
	}
}

// FACTORY
Common::ComponentBase* ComponentDestroyOnCollision::CreateComponent(TiXmlNode* p_node)
{
	return new ComponentDestroyOnCollision();
}