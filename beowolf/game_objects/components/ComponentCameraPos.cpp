//------------------------------------------------------------------------
// ComponentCharacterController
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a character controller. It polls input and sends
// movement instructions to the relevant sibling components.
//------------------------------------------------------------------------

#include "ComponentCameraPos.h"
#include "W_Model.h"
#include "GameObject.h"

using namespace week2;

//------------------------------------------------------------------------------
// Method:    ComponentCharacterController
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
ComponentCameraPos::ComponentCameraPos()
{
	isEnabled = true;
	isEnabledDirty = false;
}

//------------------------------------------------------------------------------
// Method:    ~ComponentCharacterController
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
ComponentCameraPos::~ComponentCameraPos()
{
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Polls input and sends movement instructions to relevant sibling components.
//------------------------------------------------------------------------------
void ComponentCameraPos::Update(float p_fDelta)
{
	if (glfwGetKey('C') == GLFW_PRESS)
	{
		if (!isEnabledDirty)
		{
			isEnabled = !isEnabled;
			isEnabledDirty = true;
		}
	}
	else
	{
		isEnabledDirty = false;
	}

	if (m_camera != NULL && isEnabled)
	{
		m_camera->SetPos(GetGameObject()->GetTransform().GetTranslation());
		m_camera->SetTarget(glm::vec3(0.0f, 0.0f, -1.0f) * GetGameObject()->GetTransform().GetRotation() + m_camera->GetPos());
	}
}

void ComponentCameraPos::SetCamera(Common::SceneCamera* p_camera)
{
	m_camera = p_camera;
}

void ComponentCameraPos::SetEnabled(bool setting)
{
	isEnabled = setting;
}
