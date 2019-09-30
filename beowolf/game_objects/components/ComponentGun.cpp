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
#include "ComponentGun.h"
#include <cmath>
#include "ComponentRenderableMesh.h"
#include "ComponentBobbing.h"
#include "ComponentRotating.h"
#include "ComponentSphereCollider.h"
#include "ComponentLifespan.h"
#include "EventManager.h"
#include "W_RNG.h"
#include "ComponentRigidBody.h"

using namespace week2;

//------------------------------------------------------------------------------
// Method:    ComponentCharacterController
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
ComponentGun::ComponentGun(float firerate)
{
	m_firerate = firerate;
	m_time = 0;
}

//------------------------------------------------------------------------------
// Method:    ~ComponentCharacterController
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
ComponentGun::~ComponentGun()
{
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Polls input and sends movement instructions to relevant sibling components.
//------------------------------------------------------------------------------
void ComponentGun::Update(float p_fDelta)
{
	m_time += p_fDelta;

	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if (m_time > m_firerate)
		{
			spawnBall();

			m_time = 0;
		}
	}
}

Common::GameObject* ComponentGun::spawnBall()
{
	Common::GameObject* ball = GetGameObject()->GetManager()->CreateGameObject("../resources/objectsball.xml");
	GetGameObject()->GetManager()->SetGameObjectGUID(ball, "Ball");

	glm::vec3 forcedir = GetGameObject()->GetTransform().GetRotation() * glm::vec3(0.0f, 0.0f, 1.0f);

	ball->GetTransform().SetTranslation(GetGameObject()->GetTransform().GetTranslation());
	ball->GetTransform().Translate(glm::vec3(0.0f, 4.5f, 0.0f));
	ball->GetTransform().Translate(forcedir * 3.0f);

	if (ball->GetComponent("GOC_RigidBody"))
	{
		Common::ComponentRigidBody* rigidbody = (Common::ComponentRigidBody*)ball->GetComponent("GOC_RigidBody");
		rigidbody->SetPos(ball->GetTransform().GetTranslation());
		rigidbody->ApplyForce(btVector3(forcedir.x * 1000, forcedir.y * 1000, forcedir.z * 1000));
	}

	return ball;
}

// FACTORY
Common::ComponentBase* ComponentGun::CreateComponent(TiXmlNode* p_node)
{
	float rate = 0.0f;

	for (TiXmlElement* i = p_node->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
	{
		std::string name = i->Value();

		if (name == "Firerate")
		{
			rate = std::atof(i->Attribute("value"));
		}
	}

	return new ComponentGun(rate);
}