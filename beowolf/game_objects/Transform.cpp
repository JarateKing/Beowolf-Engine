//------------------------------------------------------------------------
// Transform
//
// Created:	2012/12/12
// Author:	Carel Boers
//	
// This class implements a Transform component which is a special (mandatory)
// component that is a part of each GameObject.
//------------------------------------------------------------------------

#include <glm/gtx/transform.hpp>
#include "Transform.h"
#include "W_Types.h"

using namespace Common;

//------------------------------------------------------------------------------
// Method:    Transform
// Returns:   
// 
// Constructor
//------------------------------------------------------------------------------
Transform::Transform()
	:
	m_bDirty(true),
	m_vScale(1.0f, 1.0f, 1.0f),
	m_vTranslation(0.0f, 0.0f, 0.0f),
	m_qRotation(1.0f, 0.0f, 0.0f, 0.0f)
{
	// Calculate the initial transform
	this->GetTransformation();
}

//------------------------------------------------------------------------------
// Method:    ~Transform
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
Transform::~Transform()
{

}

//------------------------------------------------------------------------------
// Method:    Scale
// Parameter: const glm::vec3 & p_vScale
// Returns:   void
// 
// Applies a scale to the transform relative to the existing scale.
//------------------------------------------------------------------------------
void Transform::Scale(const glm::vec3& p_vScale)
{
	m_bDirty = true;
	m_vScale *= p_vScale;
}

//------------------------------------------------------------------------------
// Method:    SetScale
// Parameter: const glm::vec3 & p_vScale
// Returns:   void
// 
// Sets the scale component of the transform.
//------------------------------------------------------------------------------
void Transform::SetScale(const glm::vec3& p_vScale)
{
	m_bDirty = true;
	m_vScale = p_vScale;
}

//------------------------------------------------------------------------------
// Method:    GetScale
// Returns:   const glm::vec3&
// 
// Returns the scale component of the transform.
//------------------------------------------------------------------------------
const glm::vec3& Transform::GetScale() const
{
	return m_vScale;
}

//------------------------------------------------------------------------------
// Method:    Rotate
// Parameter: const glm::vec3 & p_vRotation
// Returns:   void
// 
// Applies a rotation to the transform relative to the existing rotation.
//------------------------------------------------------------------------------
void Transform::Rotate(const glm::vec3& p_vRotation)
{
	m_bDirty = true;

	glm::quat rot(glm::vec3(DEG_TO_RAD(p_vRotation.x), DEG_TO_RAD(p_vRotation.y), DEG_TO_RAD(p_vRotation.z)));
	m_qRotation = rot * m_qRotation;
}

//------------------------------------------------------------------------------
// Method:    SetRotation
// Parameter: const glm::vec3 & p_vRotation
// Returns:   void
// 
// Sets the rotation component of the transform.
//------------------------------------------------------------------------------
void Transform::SetRotation(const glm::vec3& p_vRotation)
{
	m_bDirty = true;

	// Reset rotation
	m_qRotation = glm::quat(glm::vec3(DEG_TO_RAD(p_vRotation.x), DEG_TO_RAD(p_vRotation.y), DEG_TO_RAD(p_vRotation.z)));
}

//------------------------------------------------------------------------------
// Method:    GetRotation
// Returns:   const glm::quat&
// 
// Gets the rotation component of the transform.
//------------------------------------------------------------------------------
const glm::quat& Transform::GetRotation() const
{
	return m_qRotation;
}

//------------------------------------------------------------------------------
// Method:    Translate
// Parameter: const glm::vec3 & p_vTranslation
// Returns:   void
// 
// Applies a translation to the transform relative to the existing position.
//------------------------------------------------------------------------------
void Transform::Translate(const glm::vec3 &p_vTranslation)
{
	m_bDirty = true;
	m_vTranslation += p_vTranslation;
}

//------------------------------------------------------------------------------
// Method:    SetTranslation
// Parameter: const glm::vec3 & p_vTranslation
// Returns:   void
// 
// Sets the translation component of the transform.
//------------------------------------------------------------------------------
void Transform::SetTranslation(const glm::vec3 &p_vTranslation)
{
	m_bDirty = true;
	m_vTranslation = p_vTranslation;
}

//------------------------------------------------------------------------------
// Method:    GetTranslation
// Returns:   const glm::vec3&
// 
// Gets the translation component of the transform.
//------------------------------------------------------------------------------
const glm::vec3& Transform::GetTranslation() const
{
	return m_vTranslation;
}

//------------------------------------------------------------------------------
// Method:    GetTransformation
// Returns:   const glm::mat4&
// 
// Gets the combined transform (scale, rotation, translation) as a 4x4 matrix.
// If any of the components have been changed since it was last calculated it 
// is recalculated, otherwise the cached value is returned.
//------------------------------------------------------------------------------
const glm::mat4& Transform::GetTransformation() const
{
	if (m_bDirty)
	{
		m_bDirty = false;
		m_mTransform = glm::mat4();
		m_mTransform = glm::translate(m_mTransform, m_vTranslation);
		m_mTransform = m_mTransform * glm::mat4_cast(m_qRotation);
		m_mTransform = glm::scale(m_mTransform, m_vScale);
	}

	return m_mTransform;
}

//------------------------------------------------------------------------------
// Method:    GetDerivedTransformation
// Returns:   const glm::mat4&
// 
// Gets the combined transform (scale, rotation, translation) as a 4x4 matrix
// including all parent transforms.
//------------------------------------------------------------------------------
const glm::mat4& Transform::GetDerivedTransformation() const
{
	// TODO CSB: get derived transformation
	return this->GetTransformation();
}

#if defined(_WIN32)

//------------------------------------------------------------------------------
// Method:    ExportToLua
// Returns:   void
// 
// Export interface to Lua.
//------------------------------------------------------------------------------
void Transform::ExportToLua()
{
	LuaPlus::LuaObject metaTable = LuaScriptManager::Instance()->GetLuaState()->GetGlobals().CreateTable("TransformMetaTable");
	metaTable.SetObject("__index", metaTable);

	metaTable.RegisterObjectDirect("Scale", (Transform*) 0, &Transform::ScaleXYZ);
	metaTable.RegisterObjectDirect("SetScale", (Transform*) 0, &Transform::SetScaleXYZ);
	metaTable.RegisterObjectDirect("Rotate", (Transform*) 0, &Transform::RotateXYZ);
	metaTable.RegisterObjectDirect("SetRotation", (Transform*) 0, &Transform::SetRotationXYZ);
	metaTable.RegisterObjectDirect("Translate", (Transform*) 0, &Transform::TranslateXYZ);
	metaTable.RegisterObjectDirect("SetTranslation", (Transform*) 0, &Transform::SetTranslationXYZ);
}

#endif