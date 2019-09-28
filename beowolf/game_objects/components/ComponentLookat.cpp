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
#include "ComponentLookat.h"
#include "GameObject.h"

using namespace week2;

//------------------------------------------------------------------------------
// Method:    ComponentCharacterController
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
ComponentLookat::ComponentLookat()
{
}

//------------------------------------------------------------------------------
// Method:    ~ComponentCharacterController
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
ComponentLookat::~ComponentLookat()
{
}

// Set what GameObject to follow
void ComponentLookat::Follow(Common::GameObject* p_target)
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
void ComponentLookat::Update(float p_fDelta)
{
	if (m_target != NULL)
	{
		GetGameObject()->GetTransform().SetRotation(glm::quat(glm::lookAt(GetGameObject()->GetTransform().GetTranslation(), m_target->GetTransform().GetTranslation() + m_offset, glm::vec3(0,1,0))));
	}
}

void ComponentLookat::SetOffset(glm::vec3 p_offset)
{
	m_offset = p_offset;
}
