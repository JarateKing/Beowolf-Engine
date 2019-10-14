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
#include "ComponentAnimController.h"
#include "ComponentCharacterController.h"
#include "ComponentRenderableMesh.h"
#include "GameObject.h"

#include <windows.h>

using namespace week2;

//------------------------------------------------------------------------------
// Method:    ComponentCharacterController
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
ComponentCharacterController::ComponentCharacterController()
{
	memset(m_bKeysDown, 0, sizeof(bool) * 256);
	memset(m_bKeysDownLast, 0, sizeof(bool) * 256);
}

//------------------------------------------------------------------------------
// Method:    ~ComponentCharacterController
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
ComponentCharacterController::~ComponentCharacterController()
{
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Polls input and sends movement instructions to relevant sibling components.
//------------------------------------------------------------------------------
void ComponentCharacterController::Update(float p_fDelta)
{
	static int prevAnim = 0;

	if (m_rigidbody == NULL)
	{
		m_rigidbody = (Common::ComponentRigidBody*)(GetGameObject()->GetComponent("GOC_RigidBody"));
	}

	// Transfer previous frame key presses to "Last" array and get current keyboard state
	for (int i = 0; i < 256; ++i)
	{
		m_bKeysDownLast[i] = m_bKeysDown[i];
		m_bKeysDown[i] = (glfwGetKey(i) == GLFW_PRESS);
	}

	if (m_rigidbody != NULL)
	{
		// Get the parent GameObject's Transform component.
		Common::Transform& transform = this->GetGameObject()->GetTransform();

		// get the animation component
		Common::ComponentBase* animator = this->GetGameObject()->GetComponent("GOC_AnimController");
		int currentAnim = prevAnim;
		glm::vec3 movement = glm::vec3(0.0f, 0.0f, 0.0f);

		// Rotate based off of input
		if (GetKeyState(VK_LEFT) & 0x8000) // LEFT
		{
			//transform.Rotate(glm::vec3(0.0f, 60.0f * p_fDelta, 0.0f));
			//m_rot = glm::vec3(0.0f, 60.0f * p_fDelta, 0.0f) * m_rot;
			m_rot = m_rot * glm::quat(glm::vec3(0.0f, 1.0f * p_fDelta, 0.0f));
		}
		if (GetKeyState(VK_RIGHT) & 0x8000) // RIGHT
		{
			//transform.Rotate(glm::vec3(0.0f, -60.0f * p_fDelta, 0.0f));
			m_rot = m_rot * glm::quat(glm::vec3(0.0f, -1.0f * p_fDelta, 0.0f));
		}

		// Move based off keyboard input
		if ((m_bKeysDown['a'] || m_bKeysDown['A']))
		{
			movement.x += 1.0f;
		}
		if ((m_bKeysDown['d'] || m_bKeysDown['D']))
		{
			movement.x += -1.0f;
		}
		if ((m_bKeysDown['w'] || m_bKeysDown['W']))
		{
			movement.z += 1.0f;
		}
		if ((m_bKeysDown['s'] || m_bKeysDown['S']))
		{
			movement.z += -1.0f;
		}

		// prevent diagonal movement being fastest
		if (glm::length(movement) > 1.0f)
			movement = glm::normalize(movement);

		// check if running
		if (GetKeyState(VK_SHIFT) & 0x8000)
		{
			currentAnim = 2;
			movement *= 20;
		}
		else
		{
			currentAnim = 1;
			movement *= 10;
		}

		// ever-present multipliers
		movement *= 1000.0f;//p_fDelta;

		if (movement == glm::vec3(0.0f, 0.0f, 0.0f))
		{
			currentAnim = 0;
			m_rigidbody->NullifyForce();
		}
		else
		{
			movement = m_rot * movement;
			m_rigidbody->ApplyForce(btVector3(movement.x, movement.y, movement.z), 15.0f);
		}

		m_rigidbody->SetAngle(btQuaternion(m_rot.x, m_rot.y, m_rot.z, m_rot.w));

		if (currentAnim != prevAnim)
		{
			if (currentAnim == 0)
			{
				((ComponentAnimController*)animator)->SetAnim("idle");
			}
			else if (currentAnim == 1)
			{
				((ComponentAnimController*)animator)->SetAnim("walking");
			}
			else if (currentAnim == 2)
			{
				((ComponentAnimController*)animator)->SetAnim("standard_run");
			}
			prevAnim = currentAnim;
		}
	}
}

// FACTORY
Common::ComponentBase* ComponentCharacterController::CreateComponent(json p_pNode)
{
	return new ComponentCharacterController();
}
