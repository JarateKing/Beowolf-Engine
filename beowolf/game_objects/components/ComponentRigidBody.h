//------------------------------------------------------------------------
// ComponentRigidBody
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a rigid body component.
//------------------------------------------------------------------------

#ifndef COMPONENTRIGIDBODY_H
#define COMPONENTRIGIDBODY_H

#include "btBulletDynamicsCommon.h"
#include "ComponentBase.h"
#include "W_Common.h"
#include "tinyxml.h"

namespace Common
{
	class ComponentRigidBody : public Common::ComponentBase
	{
	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		ComponentRigidBody();
		virtual ~ComponentRigidBody();

		virtual const std::string ComponentID(){ return std::string("GOC_RigidBody"); }
		virtual const std::string FamilyID(){ return std::string("GOC_RigidBody"); }
		virtual void Update(float p_fDelta);

		static ComponentBase* CreateComponent(TiXmlNode* p_node) { return NULL; };
		static ComponentBase* CreateComponent(TiXmlNode* p_node, GameObject* owner);

		//------------------------------------------------------------------------------
		// Public methods for "GOC_RigidBody" family of components
		//------------------------------------------------------------------------------
		virtual void Init(btCollisionShape* p_pCollisionShape, const std::string& p_strMaterial, float p_fMass, const glm::vec3& p_vOffset, bool p_bIsKinematic = false);

		virtual void SetPos(glm::vec3 pos);
		virtual void ApplyForce(btVector3 direction, float max = -1.0f);
		virtual void NullifyForce();
		virtual void SetAngle(btQuaternion angle);

		virtual void SetFlags(long type, long collides);
		virtual long GetTypeFlag();
		virtual long GetCollidesFlag();

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------

		// Bullet physics rigid body
		btRigidBody* m_pBody;

		// Collision shape
		btCollisionShape* m_pCollisionShape;

		// Offset from rigid body
		glm::vec3 m_vOffset;

		// Is Physics controlled or manually controlled
		bool m_bKinematic;

		// bitflags
		long m_eventflagsType;
		long m_eventflagsCollides;
	};
}

#endif // COMPONENTRIGIDBODY_H

