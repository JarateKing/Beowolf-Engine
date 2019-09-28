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
#include "ComponentFollow.h"
#include "GameObject.h"

using namespace week2;

//------------------------------------------------------------------------------
// Method:    ComponentCharacterController
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
ComponentFollow::ComponentFollow()
{
}

//------------------------------------------------------------------------------
// Method:    ~ComponentCharacterController
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
ComponentFollow::~ComponentFollow()
{
}

// Set what GameObject to follow
void ComponentFollow::Follow(Common::GameObject* p_target)
{
	m_target = p_target;
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Polls input and sends movement instructions to relevant sibling components.
//------------------------------------------------------------------------------
void ComponentFollow::Update(float p_fDelta)
{
	if (m_target != NULL)
	{
		GetGameObject()->GetTransform().SetTranslation(m_target->GetTransform().GetTranslation() + m_target->GetTransform().GetRotation() * m_offset);
	}
}

void ComponentFollow::SetOffset(glm::vec3 p_offset)
{
	m_offset = p_offset;
}
