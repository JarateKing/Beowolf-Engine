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
#include <sstream>

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
	BulletPhysicsManager::Instance()->GetWorld()->removeRigidBody(m_pBody);
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

Common::ComponentBase* ComponentRigidBody::CreateComponent(TiXmlNode* p_node, GameObject* owner)
{
	Common::ComponentRigidBody* rigidBody = new Common::ComponentRigidBody();

	btCollisionShape* shape;
	std::string material;
	float mass = 0;
	glm::vec3 offset = glm::vec3(0,0,0);
	bool isKinematic = false;
	long flagType = 0;
	long flagColliders = 0;

	for (TiXmlElement* i = p_node->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
	{
		std::string name = i->Value();

		if (name == "Shape")
		{
			std::stringstream values(i->Attribute("value"));
			std::string type;
			values >> type;

			if (type == "Sphere")
			{
				float radius;
				values >> radius;
				shape = new btSphereShape(radius);
			}
			else if (type == "Capsule")
			{
				float radius, height;
				values >> radius;
				values >> height;
				shape = new btCapsuleShape(radius, height);
			}
			else if (type == "Cylinder")
			{
				float x, y, z;
				values >> x;
				values >> y;
				values >> z;
				shape = new btCylinderShape(btVector3(x, y, z));
			}
			else if (type == "Plane")
			{
				float a, b, c, d;
				values >> a;
				values >> b;
				values >> c;
				values >> d;
				shape = new btStaticPlaneShape(btVector3(a, b, c), d);
			}
			else if (type == "Box")
			{
				float x, y, z;
				values >> x;
				values >> y;
				values >> z;
				shape = new btBoxShape(btVector3(x, y, z));
			}
		}
		else if (name == "Material")
		{
			material = i->Attribute("value");
		}
		else if (name == "Mass")
		{
			mass = std::atof(i->Attribute("value"));
		}
		else if (name == "Offset")
		{
			std::stringstream values(i->Attribute("value"));
			float x, y, z;
			values >> x;
			values >> y;
			values >> z;
			offset = glm::vec3(x, y, z);
		}
		else if (name == "Kinematic")
		{
			isKinematic = std::strcmp(i->Attribute("value"), "true") == 0;
		}
		else if (name == "FlagType")
		{
			flagType = std::atol(i->Attribute("value"));
		}
		else if (name == "FlagCollider")
		{
			flagColliders = std::atol(i->Attribute("value"));
		}
	}

	rigidBody->SetGameObject(owner);
	rigidBody->Init(shape, material, mass, offset, isKinematic);
	rigidBody->SetFlags(flagType, flagColliders);
	return rigidBody;
}

void ComponentRigidBody::SetPos(glm::vec3 pos)
{
	btTransform transform = m_pBody->getWorldTransform();
	transform.setOrigin(btVector3(pos.x, pos.y, pos.z));
	m_pBody->setWorldTransform(transform);
}

void ComponentRigidBody::ApplyForce(btVector3 direction, float max)
{
	m_pBody->activate(true);
	if (m_pBody->getLinearVelocity() == btVector3(0,0,0) || max < 0.0f)
	{
		m_pBody->applyCentralForce(direction);
	}
	else
	{
		btVector3 newvec = direction + m_pBody->getLinearVelocity();
		float newlength = newvec.length();
		if (newlength > max)
		{
			m_pBody->setLinearVelocity(newvec.normalized() * max);
		}
		else
		{
			m_pBody->setLinearVelocity(newvec);
		}
	}
}

void ComponentRigidBody::NullifyForce()
{
	m_pBody->activate(true);
	m_pBody->setLinearVelocity(btVector3(0, 0, 0));
}

void ComponentRigidBody::SetAngle(btQuaternion angle)
{
	m_pBody->activate(true);
	m_pBody->setAngularVelocity(btVector3(0,0,0));

	btTransform transform = m_pBody->getWorldTransform();
	transform.setRotation(angle);
	m_pBody->setWorldTransform(transform);
}

void ComponentRigidBody::SetFlags(long type, long collides)
{
	m_eventflagsType = type;
	m_eventflagsCollides = collides;
}

long ComponentRigidBody::GetTypeFlag()
{
	return m_eventflagsType;
}

long ComponentRigidBody::GetCollidesFlag()
{
	return m_eventflagsCollides;
}
