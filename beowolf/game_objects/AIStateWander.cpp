//------------------------------------------------------------------------
// AIStateWander
//
// Created:	2013/02/10
// Author:	Carel Boers
//	
// Wander behaviour state.
//------------------------------------------------------------------------

#include "AIStateWander.h"
#include "ComponentBase.h"
#include "ComponentAnimController.h"
#include "GameObject.h"
#include <glm/gtx/vector_angle.hpp>
#include "ComponentRigidBody.h"
#include "ComponentStunOnCollision.h"

using namespace week9;

//------------------------------------------------------------------------------
// Method:    AIStateWander
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
AIStateWander::AIStateWander()
{
}

//------------------------------------------------------------------------------
// Method:    ~AIStateWander
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
AIStateWander::~AIStateWander()
{
}

//------------------------------------------------------------------------------
// Method:    Enter
// Returns:   void
// 
// Called when this state becomes active.
//------------------------------------------------------------------------------
void AIStateWander::Enter()
{
	// Trigger the injured walk animation
	ComponentAIController* pController = static_cast<ComponentAIController*>(m_pStateMachine->GetStateMachineOwner());
	Common::ComponentBase* pComponent = pController->GetGameObject()->GetComponent("GOC_AnimController");
	if (pComponent)
	{
		week2::ComponentAnimController* pAnimController = static_cast<week2::ComponentAnimController*>(pComponent);
		pAnimController->SetAnim("walk_injured");
	}

	// - - - - - - -- - 
	// Choose a destination

	m_vDestination = ChooseDestination(pController);
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Called each from when this state is active.
//------------------------------------------------------------------------------
void AIStateWander::Update(float p_fDelta)
{
	ComponentAIController* pController = static_cast<ComponentAIController*>(m_pStateMachine->GetStateMachineOwner());
	Common::Transform& transform = pController->GetGameObject()->GetTransform();

	float moddedDelta = p_fDelta;
	week2::ComponentStunOnCollision* stunComponent = (week2::ComponentStunOnCollision*)(pController->GetGameObject()->GetComponent("GOC_StunOnCollision"));
	if (stunComponent != NULL)
	{
		if (stunComponent->isStunned())
		{
			moddedDelta /= 2;
		}
	}

	// Check how close we are to our destination
	glm::vec3 vDiff = m_vDestination - transform.GetTranslation();
	while (glm::length(vDiff) < 2.5f)
	{
		// close enough, pick a new spot
		m_vDestination = ChooseDestination(pController);
		vDiff = m_vDestination - transform.GetTranslation();
	}

	Common::ComponentRigidBody* rigidbody = (Common::ComponentRigidBody*)(pController->GetGameObject()->GetComponent("GOC_RigidBody"));

	if (rigidbody != NULL)
	{
		if (glm::length(vDiff) > 0.0f)
		{
			// Move towards our destination
			vDiff = glm::normalize(vDiff);
			float fVelocity = 4.0f;
			glm::vec3 movement = vDiff * fVelocity * moddedDelta;
			rigidbody->ApplyForce(btVector3(movement.x, movement.y, movement.z), 4.0f);

			// Rotate facing direction
			float fAngle = glm::atan(vDiff.x, vDiff.z);
			glm::quat rot = glm::quat(glm::vec3(0.0f, fAngle, 0.0f));
			rigidbody->SetAngle(btQuaternion(rot.x, rot.y, rot.z, rot.w));
		}
		else
		{
			rigidbody->NullifyForce();
		}
	}
}

//------------------------------------------------------------------------------
// Method:    Exit
// Returns:   void
// 
// Called when this state becomes inactive.
//------------------------------------------------------------------------------
void AIStateWander::Exit()
{	
	// Nothing to do when exiting this state
}

glm::vec3 AIStateWander::ChooseDestination(ComponentAIController* controller)
{
	const glm::vec3& vPosition = controller->GetGameObject()->GetTransform().GetTranslation();

	// Random offset between (5,0,0) and (10,0,0)
	glm::vec4 vDestOffset = glm::vec4(5.0f + (rand() % 5), 0.0f, 0.0f, 0.0f);

	// Rotate our offset by a random amount
	float fRotation = (float)(rand() % 360);
	glm::mat4 mRotation;
	mRotation = glm::rotate(mRotation, fRotation, glm::vec3(0.0, 1.0, 0.0));
	vDestOffset = mRotation * vDestOffset;

	// Add our rotated offset to our position to get our random destination
	return vPosition + glm::vec3(vDestOffset.x, vDestOffset.y, vDestOffset.z);
}