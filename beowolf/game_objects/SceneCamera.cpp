//------------------------------------------------------------------------
// SceneCamera
//
// Created:	2012/12/23
// Author:	Carel Boers
//	
// Abstraction of the underlying OpenGL view/projection matrices.
//------------------------------------------------------------------------

#include "SceneCamera.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace Common;

//------------------------------------------------------------------------------
// Method:    SceneCamera
// Returns:   
// 
// Constructor with default initialization
//------------------------------------------------------------------------------
SceneCamera::SceneCamera()
	:
	m_fFOV(45.0f),
	m_fAspectRatio(1280.0f/720.0f),
	m_fNearClip(0.1f),
	m_fFarClip(1000.0f),
	m_vPos(glm::vec3(0.0f, 0.0f, -5.0f)),
	m_vTarget(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_vUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_bProjectionDirty(true),
	m_bViewDirty(true)
{
}

//------------------------------------------------------------------------------
// Method:    SceneCamera
// Parameter: float p_fFOV
// Parameter: float p_fAspectRatio
// Parameter: float p_fNearClip
// Parameter: float p_fFarClip
// Parameter: const glm::vec3 & p_vPos
// Parameter: const glm::vec3 & p_vTarget
// Parameter: const glm::vec3 & p_vUp
// Returns:   
// 
// Constructor with explicit initialization of members
//------------------------------------------------------------------------------
SceneCamera::SceneCamera(float p_fFOV, float p_fAspectRatio, float p_fNearClip, float p_fFarClip, const glm::vec3 &p_vPos, const glm::vec3& p_vTarget, const glm::vec3& p_vUp)
	:
	m_fFOV(p_fFOV),
	m_fAspectRatio(p_fAspectRatio),
	m_fNearClip(p_fNearClip),
	m_fFarClip(p_fFarClip),
	m_vPos(p_vPos),
	m_vTarget(p_vTarget),
	m_vUp(p_vUp),
	m_bProjectionDirty(true),
	m_bViewDirty(true)	
{
}

//------------------------------------------------------------------------------
// Method:    ~SceneCamera
// Returns:   
// 
// Destructor
//------------------------------------------------------------------------------
SceneCamera::~SceneCamera()
{
}

//------------------------------------------------------------------------------
// Method:    SetFOV
// Parameter: float p_fFOV
// Returns:   void
// 
// Set's the camera's FOV.
//------------------------------------------------------------------------------
void SceneCamera::SetFOV(float p_fFOV)
{
	m_bProjectionDirty = true;
	m_fFOV = p_fFOV;
}

//------------------------------------------------------------------------------
// Method:    GetFOV
// Returns:   float
// 
// Returns the camera's FOV.
//------------------------------------------------------------------------------
float SceneCamera::GetFOV() const
{
	return m_fFOV;
}

//------------------------------------------------------------------------------
// Method:    SetAspectRatio
// Parameter: float p_fAspectRatio
// Returns:   void
// 
// Set's the camera's FOV.
//------------------------------------------------------------------------------
void SceneCamera::SetAspectRatio(float p_fAspectRatio)
{
	m_bProjectionDirty = true;
	m_fAspectRatio = p_fAspectRatio;
}

//------------------------------------------------------------------------------
// Method:    GetAspectRatio
// Returns:   float
// 
// Returns the camera's FOV.
//------------------------------------------------------------------------------
float SceneCamera::GetAspectRatio() const
{
	return m_fAspectRatio;
}

//------------------------------------------------------------------------------
// Method:    SetNearClip
// Parameter: float p_fNearClip
// Returns:   void
// 
// Set's the camera's near clip plane.
//------------------------------------------------------------------------------
void SceneCamera::SetNearClip(float p_fNearClip)
{
	m_bProjectionDirty = true;
	m_fNearClip = p_fNearClip;
}

//------------------------------------------------------------------------------
// Method:    GetNearClip
// Returns:   float
// 
// Returns the camera's near clip plane.
//------------------------------------------------------------------------------
float SceneCamera::GetNearClip() const
{
	return m_fNearClip;
}

//------------------------------------------------------------------------------
// Method:    SetFarClip
// Parameter: float p_fFarClip
// Returns:   void
// 
// Set's the camera's far clip plane
//------------------------------------------------------------------------------
void SceneCamera::SetFarClip(float p_fFarClip)
{
	m_bProjectionDirty = true;
	m_fFarClip = p_fFarClip;
}

//------------------------------------------------------------------------------
// Method:    GetFarClip
// Returns:   float
// 
// Returns the camera's far clip plane.
//------------------------------------------------------------------------------
float SceneCamera::GetFarClip() const
{
	return m_fFarClip;
}

//------------------------------------------------------------------------------
// Method:    SetPos
// Parameter: const glm::vec3 & p_vPos
// Returns:   void
// 
// Sets the position of the camera
//------------------------------------------------------------------------------
void SceneCamera::SetPos(const glm::vec3 &p_vPos)
{
	m_bViewDirty = true;
	m_vPos = p_vPos;
}

//------------------------------------------------------------------------------
// Method:    GetPos
// Returns:   const glm::vec3&
// 
// Gets the position of the camera.
//------------------------------------------------------------------------------
const glm::vec3& SceneCamera::GetPos() const
{
	return m_vPos;
}

//------------------------------------------------------------------------------
// Method:    SetTarget
// Parameter: const glm::vec3 & p_vTarget
// Returns:   void
// 
// Sets the target position for the camera to look at.
//------------------------------------------------------------------------------
void SceneCamera::SetTarget(const glm::vec3& p_vTarget)
{
	m_bViewDirty = true;
	m_vTarget = p_vTarget;
}

//------------------------------------------------------------------------------
// Method:    GetTarget
// Returns:   const glm::vec3&
// 
// Gets the target position for the camera to look at.
//------------------------------------------------------------------------------
const glm::vec3& SceneCamera::GetTarget() const
{
	return m_vTarget;
}

//------------------------------------------------------------------------------
// Method:    SetUp
// Parameter: const glm::vec3 & p_vUp
// Returns:   void
// 
// Sets the "up" vector for the camera.
//------------------------------------------------------------------------------
void SceneCamera::SetUp(const glm::vec3& p_vUp)
{
	m_bViewDirty = true;
	m_vUp = p_vUp;
}

//------------------------------------------------------------------------------
// Method:    GetUp
// Returns:   const glm::vec3&
// 
// Gets the "up" vector for the camera.
//------------------------------------------------------------------------------
const glm::vec3& SceneCamera::GetUp() const
{
	return m_vUp;
}

//------------------------------------------------------------------------------
// Method:    GetProjectionMatrix
// Returns:   const glm::mat4&
// 
// Returns the projection matrix, recalculating it as needed.
//------------------------------------------------------------------------------
const glm::mat4& SceneCamera::GetProjectionMatrix() const
{
	if (m_bProjectionDirty)
	{
		m_mProjectionMatrix = glm::perspective(m_fFOV, m_fAspectRatio, m_fNearClip, m_fFarClip);
		m_bProjectionDirty = false;
	}
	return m_mProjectionMatrix;
}

//------------------------------------------------------------------------------
// Method:    GetViewMatrix
// Returns:   const glm::mat4&
// 
// Returns the view matrix, recalculating it as needed.
//------------------------------------------------------------------------------
const glm::mat4& SceneCamera::GetViewMatrix() const
{
	if (m_bViewDirty)
	{
		m_mViewMatrix = glm::lookAt(m_vPos, m_vTarget, m_vUp);
		m_bViewDirty = false;
	}
	return m_mViewMatrix;
}