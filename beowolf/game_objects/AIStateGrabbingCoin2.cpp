//------------------------------------------------------------------------
// AIStateGrabbingCoin2
//
// Created:	2013/02/10
// Author:	Carel Boers
//	
// Idle behaviour state.
//------------------------------------------------------------------------

#include "AIPathfinder.h"
#include "AIStateGrabbingCoin2.h"
#include "ComponentBase.h"
#include "ComponentAIController.h"
#include "ComponentAnimController.h"
#include "GameObject.h"
#include <glm/gtx/vector_angle.hpp>
#include "ComponentRigidBody.h"
#include "ComponentStunOnCollision.h"

using namespace week9;

//------------------------------------------------------------------------------
// Method:    AIStateGrabbingCoin2
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
AIStateGrabbingCoin2::AIStateGrabbingCoin2()
{
}

//------------------------------------------------------------------------------
// Method:    ~AIStateGrabbingCoin2
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
AIStateGrabbingCoin2::~AIStateGrabbingCoin2()
{
}

//------------------------------------------------------------------------------
// Method:    Enter
// Returns:   void
// 
// Called when this state becomes active.
//------------------------------------------------------------------------------
void AIStateGrabbingCoin2::Enter()
{
	// Trigger the walk animation
	ComponentAIController* pController = static_cast<ComponentAIController*>(m_pStateMachine->GetStateMachineOwner());
	Common::ComponentBase* pComponent = pController->GetGameObject()->GetComponent("GOC_AnimController");
	if (pComponent)
	{
		week2::ComponentAnimController* pAnimController = static_cast<week2::ComponentAnimController*>(pComponent);
		pAnimController->SetAnim("walk");
	}

	// Set our target
	// NOTE: this really should come into the state Enter() method as a context parameter so it's not hard coded.
	m_pTargetGameObject = pController->GetGameObject()->GetManager()->GetGameObject("Coin2");
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Called each from when this state is active.
//------------------------------------------------------------------------------
void AIStateGrabbingCoin2::Update(float p_fDelta)
{
	if (m_pTargetGameObject)
	{
		ComponentAIController* pController = static_cast<ComponentAIController*>(m_pStateMachine->GetStateMachineOwner());
		Common::Transform& transform = pController->GetGameObject()->GetTransform();
		Common::Transform& targetTransform = m_pTargetGameObject->GetTransform();

		float moddedDelta = p_fDelta;

		week2::ComponentStunOnCollision* stunComponent = (week2::ComponentStunOnCollision*)(pController->GetGameObject()->GetComponent("GOC_StunOnCollision"));
		if (stunComponent != NULL)
		{
			if (stunComponent->isStunned())
			{
				moddedDelta /= 2;
			}
		}

		// In two cases we want to recalculate our path:
		//	1. If we don't have a path
		//  2. If our target position is different from the existing path we are following
		if (m_lPath.empty())
		{
			// We don't have a path yet
			m_lPath = AIPathfinder::Instance()->FindPath(transform.GetTranslation(), targetTransform.GetTranslation());
		}
		else
		{
			const AIPathfinder::PathNode* const pEndNode = AIPathfinder::Instance()->GetClosestNode(targetTransform.GetTranslation());
			glm::vec3 vTarget = m_lPath.back();
			if (pEndNode->m_vPosition != vTarget || AIPathfinder::Instance()->IsNodeBlocked(m_lPath.front()))
			{
				// Our path has changed
				m_lPath = AIPathfinder::Instance()->FindPath(transform.GetTranslation(), targetTransform.GetTranslation());
			}
		}

		// Find the next node we should move towards
		glm::vec3 vMoveTarget = transform.GetTranslation();
		glm::vec3 currentPos = transform.GetTranslation();
		int iters = 0;
		currentPos.y = 0;
		while (iters < 2)
		{
			iters += 10; // exit out if it's not subtracted by 9 later
			if (m_lPath.size() > 0)
			{
				// Move towards the first node in the path
				AIPathfinder::PositionList::const_iterator it = m_lPath.begin();
				vMoveTarget = static_cast<glm::vec3>(*it);
				if (AIPathfinder::Instance()->IsNodeBlocked(vMoveTarget))
				{
					iters -= 9;
					m_lPath = AIPathfinder::Instance()->FindPath(transform.GetTranslation(), targetTransform.GetTranslation());
				}
				else
				{
					AIPathfinder::Instance()->BlockNode(vMoveTarget);
					vMoveTarget.y = 0;

					// If we're here then choose then next node instead
					if (m_lPath.size() > 1 && glm::length(vMoveTarget - currentPos) < 0.5f)
					{
						m_lPath.pop_front();
						it = m_lPath.begin();
						vMoveTarget = static_cast<glm::vec3>(*it);

						if (AIPathfinder::Instance()->IsNodeBlocked(vMoveTarget))
						{
							iters -= 9;
							m_lPath = AIPathfinder::Instance()->FindPath(transform.GetTranslation(), targetTransform.GetTranslation());
						}
						else
						{
							AIPathfinder::Instance()->BlockNode(vMoveTarget);
							vMoveTarget.y = 0;
						}
					}
				}
			}
		}

		// Move towards our destination
		glm::vec3 vDiff = vMoveTarget - currentPos;
		Common::ComponentRigidBody* rigidbody = (Common::ComponentRigidBody*)(pController->GetGameObject()->GetComponent("GOC_RigidBody"));
		if (rigidbody != NULL)
		{
			if (glm::length(vDiff) > 0.0f)
			{
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
void AIStateGrabbingCoin2::Exit()
{	
	m_pTargetGameObject = NULL;
}