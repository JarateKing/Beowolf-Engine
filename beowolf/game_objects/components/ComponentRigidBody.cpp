//------------------------------------------------------------------------
// ComponentRigidBody
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a rigid body component.
//--------------------------------------------------------------------

#include "BulletPhysicsManager.h"
#include "ComponentRigidBody.h"
#include "GameObject.h"

using namespace Common;

//------------------------------------------------------------------------------
// Method:    ComponentRigidBody
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
ComponentRigidBody::ComponentRigidBody()
	:
	m_pBody(NULL),
	m_pCollisionShape(NULL),
	m_bKinematic(false)
{
}

//------------------------------------------------------------------------------
// Method:    ~ComponentRigidBody
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
ComponentRigidBody::~ComponentRigidBody()
{
}

//------------------------------------------------------------------------------
// Method:    Init
// Parameter: btCollisionShape * p_pCollisionShape
// Parameter: const std::string & p_strMaterial
// Parameter: float p_fMass
// Parameter: bool p_bIsKinematic
// Returns:   void
// 
// Initializes the rigid body component.
//------------------------------------------------------------------------------
void ComponentRigidBody::Init(btCollisionShape* p_pCollisionShape, const std::string& p_strMaterial, float p_fMass, const glm::vec3& p_vOffset, bool p_bIsKinematic)
{
	m_pCollisionShape = p_pCollisionShape;
	m_bKinematic = p_bIsKinematic;
	m_vOffset = p_vOffset;

	// Set mass
	btScalar mass(p_fMass);
	bool isDynamic = (mass != 0.f);
	btVector3 localInertia(0,0,0);
	if (isDynamic)
	{
		m_pCollisionShape->calculateLocalInertia(mass, localInertia);
	}

	// Set initial transform
	Transform& transform = this->GetGameObject()->GetTransform();
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(transform.GetTranslation().x, transform.GetTranslation().y, transform.GetTranslation().z));

	// Setup the motion state
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, m_pCollisionShape, localInertia);

	// Material specific properties
	BulletPhysicsMaterialManager::PhysicsMaterial* pMaterial = BulletPhysicsManager::Instance()->GetMaterial(p_strMaterial);
	if (pMaterial)
	{
		rbInfo.m_restitution = pMaterial->restitution;
		rbInfo.m_friction = pMaterial->friction;
	}

	m_pBody = new btRigidBody(rbInfo);
	m_pBody->setUserPointer(this->GetGameObject());

	// Set kinematic flags if this rigid body is manually positioned
	if (m_bKinematic)
	{
		m_pBody->setCollisionFlags( m_pBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		m_pBody->setActivationState(DISABLE_DEACTIVATION);
	}

	// Add rigid body to the world
	BulletPhysicsManager::Instance()->GetWorld()->addRigidBody(m_pBody);
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Update rigid body component.
//------------------------------------------------------------------------------
void ComponentRigidBody::Update(float p_fDelta)
{
	if (m_bKinematic)
	{
		// Get the rigid body transform
		btTransform trans;
		m_pBody->getMotionState()->getWorldTransform(trans);

		// Apply to the rigid body
		const glm::vec3& vPosition = this->GetGameObject()->GetTransform().GetTranslation();
		const glm::quat& qRotation = this->GetGameObject()->GetTransform().GetRotation();
		trans.setOrigin(btVector3(vPosition.x, vPosition.y, vPosition.z) - btVector3(m_vOffset.x, m_vOffset.y, m_vOffset.z));
		trans.setRotation(btQuaternion(qRotation.x, qRotation.y, qRotation.z, qRotation.w));
		m_pBody->getMotionState()->setWorldTransform(trans);
	}
	else
	{
		// Get the rigid body transform
		btTransform trans;
		m_pBody->getMotionState()->getWorldTransform(trans);

		// Rotation and translation
		glm::quat qRot = glm::quat(trans.getRotation().getW(), trans.getRotation().getX(), trans.getRotation().getY(), trans.getRotation().getZ());
		glm::vec3 vPos = glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
		glm::vec3 vOffset = glm::mat3_cast(qRot) * m_vOffset;

		// Apply to the game object
		Transform& transform = this->GetGameObject()->GetTransform();
		transform.SetTranslation(vPos + vOffset);
		transform.SetRotation(qRot);
	}
}