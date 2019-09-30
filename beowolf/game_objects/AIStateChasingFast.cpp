//------------------------------------------------------------------------
// AIStateChasingFast
//
// Created:	2013/02/10
// Author:	Carel Boers
//	
// Fast chase behaviour state.
//------------------------------------------------------------------------

#include "AIStateChasingFast.h"
#include "ComponentBase.h"
#include "ComponentAIController.h"
#include "ComponentAnimController.h"
#include "GameObject.h"
#include <glm/gtx/vector_angle.hpp>

using namespace week9;

//------------------------------------------------------------------------------
// Method:    AIStateChasingFast
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
AIStateChasingFast::AIStateChasingFast()
	:
	m_pTargetGameObject(NULL)
{	
}

//------------------------------------------------------------------------------
// Method:    ~AIStateChasingFast
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
AIStateChasingFast::~AIStateChasingFast()
{
}

//------------------------------------------------------------------------------
// Method:    Enter
// Returns:   void
// 
// Called when this state becomes active.
//------------------------------------------------------------------------------
void AIStateChasingFast::Enter()
{
	// Trigger the run animation
	ComponentAIController* pController = static_cast<ComponentAIController*>(m_pStateMachine->GetStateMachineOwner());
	Common::ComponentBase* pComponent = pController->GetGameObject()->GetComponent("GOC_AnimController");
	if (pComponent)
	{
		week2::ComponentAnimController* pAnimController = static_cast<week2::ComponentAnimController*>(pComponent);
		pAnimController->SetAnim("run");
	}

	// Set our target
	// NOTE: this really should come into the state Enter() method as a context parameter so it's not hard coded.
	m_pTargetGameObject = pController->GetGameObject()->GetManager()->GetGameObject("PlayerCharacter");
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Called each from when this state is active.
//------------------------------------------------------------------------------
void AIStateChasingFast::Update(float p_fDelta)
{
	if (m_pTargetGameObject)
	{
		ComponentAIController* pController = static_cast<ComponentAIController*>(m_pStateMachine->GetStateMachineOwner());
		Common::Transform& transform = pController->GetGameObject()->GetTransform();
		Common::Transform& targetTransform = m_pTargetGameObject->GetTransform();

		// Move towards our destination
		glm::vec3 vDiff = targetTransform.GetTranslation() - transform.GetTranslation();
		vDiff = glm::normalize(vDiff);
		float fVelocity = 8.0f;
		transform.Translate(vDiff * fVelocity * p_fDelta);

		// Rotate facing direction
		float fAngle = glm::orientedAngle(glm::vec3(0.0f, 0.0f, 1.0f), vDiff, glm::vec3(0.0f, 1.0f, 0.0f));
		transform.SetRotationXYZ(0.0f, fAngle, 0.0f);
	}
	else
	{
		// If we don't have a target, we shouldn't be in a chase state; go to idle
		m_pStateMachine->GoToState(ComponentAIController::eAIState_Idle);
	}
}

//------------------------------------------------------------------------------
// Method:    Exit
// Returns:   void
// 
// Called when this state becomes inactive.
//------------------------------------------------------------------------------
void AIStateChasingFast::Exit()
{	
	m_pTargetGameObject = NULL;
}