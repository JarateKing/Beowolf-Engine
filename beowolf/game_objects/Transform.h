//------------------------------------------------------------------------
// Transform
//
// Created:	2012/12/12
// Author:	Carel Boers
//	
// This class implements a Transform component which is a special (mandatory)
// component that is a part of each GameObject.
//------------------------------------------------------------------------

#ifndef TRANSFORM
#define TRANSFORM

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#if defined(_WIN32)
#include "LuaScriptManager.h"
#endif

namespace Common
{
	class Transform
	{
	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		Transform();
		virtual ~Transform();

		// Scale
		void Scale(const glm::vec3& p_vScale);
		void ScaleXYZ(float x, float y, float z) { this->Scale(glm::vec3(x, y, z)); }
		void SetScale(const glm::vec3& p_vScale);
		void SetScaleXYZ(float x, float y, float z) { this->SetScale(glm::vec3(x, y, z)); }
		const glm::vec3& GetScale() const;

		// Rotate
		void Rotate(const glm::vec3& p_vRotation);
		void RotateXYZ(float x, float y, float z) { this->Rotate(glm::vec3(x, y, z)); }
		void SetRotation(const glm::vec3& p_vRotation);
		void SetRotation(const glm::quat& p_qRotation) { m_qRotation = p_qRotation; }
		void SetRotationXYZ(float x, float y, float z) { this->SetRotation(glm::vec3(x, y, z)); }
		const glm::quat& GetRotation() const;

		// Translate
		void Translate(const glm::vec3& p_vTranslation);
		void TranslateXYZ(float x, float y, float z) { this->Translate(glm::vec3(x, y, z)); }
		void SetTranslation(const glm::vec3 &p_vTranslation);
		void SetTranslationXYZ(float x, float y, float z) { this->SetTranslation(glm::vec3(x, y, z)); }
		const glm::vec3& GetTranslation() const;

		// Transformation and DerivedTransformation
		const glm::mat4& GetTransformation() const;
		const glm::mat4& GetDerivedTransformation() const;

		// Lua bindings
		static void ExportToLua();

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------

		// Whether the transformation matrix needs to be recalculated because the transform has changed
		mutable bool m_bDirty;

		// The scale, rotation and translation
		glm::vec3 m_vScale;
		glm::vec3 m_vTranslation;
		glm::quat m_qRotation;


		// The combined transform
		mutable glm::mat4 m_mTransform;
	};
}

#endif // TRANSFORM