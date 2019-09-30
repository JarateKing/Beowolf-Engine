//------------------------------------------------------------------------
// AIStateIdle
//
// Created:	2013/02/10
// Author:	Carel Boers
//	
// Idle behaviour state.
//------------------------------------------------------------------------

#include "AIStateIdle.h"
#include "ComponentBase.h"
#include "ComponentAIController.h"
#include "ComponentAnimController.h"
#include "GameObject.h"

using namespace week9;

//------------------------------------------------------------------------------
// Method:    AIStateIdle
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
AIStateIdle::AIStateIdle()
{
}

//------------------------------------------------------------------------------
// Method:    ~AIStateIdle
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
AIStateIdle::~AIStateIdle()
{
}

//------------------------------------------------------------------------------
// Method:    Enter
// Returns:   void
// 
// Called when this state becomes active.
//------------------------------------------------------------------------------
void AIStateIdle::Enter()
{
	// Trigger the idle animation
	ComponentAIController* pController = static_cast<ComponentAIController*>(m_pStateMachine->GetStateMachineOwner());
	Common::ComponentBase* pComponent = pController->GetGameObject()->GetComponent("GOC_AnimController");
	if (pComponent)
	{
		week2::ComponentAnimController* pAnimController = static_cast<week2::ComponentAnimController*>(pComponent);
		pAnimController->SetAnim("idle");
	}
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Called each from when this state is active.
//------------------------------------------------------------------------------
void AIStateIdle::Update(float p_fDelta)
{
	// Nothing to do in idle
}

//------------------------------------------------------------------------------
// Method:    Exit
// Returns:   void
// 
// Called when this state becomes inactive.
//------------------------------------------------------------------------------
void AIStateIdle::Exit()
{	
	// Nothing to do in order to clean up idling
}