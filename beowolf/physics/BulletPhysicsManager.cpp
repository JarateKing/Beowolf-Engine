//------------------------------------------------------------------------
// BulletPhysicsManager
//
// Created:	2013/02/17
// Author:	Carel Boers
//	
// Manager to handle integration and initialization with Bullet.
//------------------------------------------------------------------------

#include "BulletPhysicsManager.h"
#include "GameObject.h"
#include <cassert>

using namespace Common;

// Static singleton instance
BulletPhysicsManager* BulletPhysicsManager::s_pPhysicsManagerInstance = NULL;

//------------------------------------------------------------------------------
// Method:    CreateInstance
// Returns:   void
// 
// Creates the singleton instance.
//------------------------------------------------------------------------------
void BulletPhysicsManager::CreateInstance(const char* p_strMaterialsPath, const char* p_strDebugLinesVertexShader, const char* p_strDebugLinesFragShader)
{
	assert(s_pPhysicsManagerInstance == NULL);
	s_pPhysicsManagerInstance = new BulletPhysicsManager(p_strMaterialsPath, p_strDebugLinesVertexShader, p_strDebugLinesFragShader);
}

//------------------------------------------------------------------------------
// Method:    DestroyInstance
// Returns:   void
// 
// Destroys the singleton instance.
//------------------------------------------------------------------------------
void BulletPhysicsManager::DestroyInstance()
{
	assert(s_pPhysicsManagerInstance != NULL);
	delete s_pPhysicsManagerInstance;
	s_pPhysicsManagerInstance = NULL;
}

//------------------------------------------------------------------------------
// Method:    Instance
// Returns:   BulletPhysicsManager::BulletPhysicsManager*
// 
// Access to singleton instance.
//------------------------------------------------------------------------------
BulletPhysicsManager* BulletPhysicsManager::Instance()
{
	assert(s_pPhysicsManagerInstance);
	return s_pPhysicsManagerInstance;
}

//------------------------------------------------------------------------------
// Method:    BulletPhysicsManager
// Parameter: const char * p_strMaterialsPath
// Parameter: const char * p_strDebugLinesVertexShader
// Parameter: const char * p_strDebugLinesFragShader
// Returns:   
// 
// Constructor
//------------------------------------------------------------------------------
BulletPhysicsManager::BulletPhysicsManager(const char* p_strMaterialsPath, const char* p_strDebugLinesVertexShader, const char* p_strDebugLinesFragShader)
	:
	m_pCollisionConfiguration(NULL),
	m_pDispatcher(NULL),
	m_pOverlappingPairCache(NULL),
	m_pSolver(NULL),
	m_pDynamicsWorld(NULL),
	m_pMaterialManager(NULL),
	m_bRenderDebug(false),
	m_pLineDrawer(NULL),
	m_pDebugDrawer(NULL)
{
	// Initialize Bullet Physics members.
	m_pCollisionConfiguration = new btDefaultCollisionConfiguration();
	m_pDispatcher = new	btCollisionDispatcher(m_pCollisionConfiguration);
	m_pOverlappingPairCache = new btDbvtBroadphase();
	m_pSolver = new btSequentialImpulseConstraintSolver;
	m_pDynamicsWorld = new btDiscreteDynamicsWorld(m_pDispatcher, m_pOverlappingPairCache, m_pSolver, m_pCollisionConfiguration);

	// Set world gravity.
	m_pDynamicsWorld->setGravity(btVector3(0,-10,0));

	// Load materials
	m_pMaterialManager = new BulletPhysicsMaterialManager();
	m_pMaterialManager->LoadMaterials(p_strMaterialsPath);

	// Debug drawer for physics
	if (p_strDebugLinesVertexShader && p_strDebugLinesFragShader)
	{
		m_pLineDrawer = new wolf::LineDrawer();
		m_pLineDrawer->Init(p_strDebugLinesVertexShader, p_strDebugLinesFragShader);
		m_pDebugDrawer = new BulletPhysicsDebugDrawer(m_pLineDrawer);
		m_pDynamicsWorld->setDebugDrawer(m_pDebugDrawer);
	}

	// Set ourselves as the world user info in order to cast back inside the TickCallback().
	m_pDynamicsWorld->setWorldUserInfo(this);

	// Register for tick callbacks
	m_pDynamicsWorld->setInternalTickCallback(TickCallback);
}

//------------------------------------------------------------------------------
// Method:    ~BulletPhysicsManager
// Parameter: void
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
BulletPhysicsManager::~BulletPhysicsManager()
{
	// Destroy debug drawer
	delete m_pDebugDrawer;
	m_pDebugDrawer = NULL;
	delete m_pLineDrawer;
	m_pLineDrawer = NULL;
	m_pDynamicsWorld->setDebugDrawer(NULL);

	// Cleanup rigid bodies
	for (int i = m_pDynamicsWorld->getNumCollisionObjects()-1; i >= 0 ; --i)
	{
		btCollisionObject* pObj = m_pDynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* pBody = btRigidBody::upcast(pObj);
		if (pBody && pBody->getMotionState())
		{
			delete pBody->getMotionState();
		}
		m_pDynamicsWorld->removeCollisionObject(pObj);
		delete pObj;
	}

	// Delete Bullet Physics members
	delete m_pDynamicsWorld;
	m_pDynamicsWorld = NULL;
	delete m_pSolver;
	m_pSolver = NULL;
	delete m_pOverlappingPairCache;
	m_pOverlappingPairCache = NULL;
	delete m_pDispatcher;
	m_pDispatcher = NULL;
	delete m_pCollisionConfiguration;
	m_pCollisionConfiguration = NULL;

	// Delete material manager
	delete m_pMaterialManager;
	m_pMaterialManager = NULL;
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Updates Bullet physics SDK and runs physics simulation
//------------------------------------------------------------------------------
void BulletPhysicsManager::Update(float p_fDelta)
{
	m_pDynamicsWorld->stepSimulation(p_fDelta, 10);
}

//------------------------------------------------------------------------------
// Method:    Render
// Parameter: const glm::mat4 & p_mProj
// Parameter: const glm::mat4 & p_mView
// Returns:   void
// 
// Render physics debugging info.
//------------------------------------------------------------------------------
void BulletPhysicsManager::Render(const glm::mat4& p_mProj, const glm::mat4& p_mView)
{
	m_pDynamicsWorld->debugDrawWorld();
	m_pLineDrawer->Render(p_mProj, p_mView);
}

//------------------------------------------------------------------------------
// Method:    ToggleDebugRendering
// Returns:   void
// 
// Toggle debug rendering.
//------------------------------------------------------------------------------
void BulletPhysicsManager::ToggleDebugRendering()
{ 
	m_bRenderDebug = !m_bRenderDebug; 
	m_pDebugDrawer->setDebugMode(m_bRenderDebug ? 1 : 0);
}

//------------------------------------------------------------------------------
// Method:    GetMaterial
// Parameter: const std::string & p_strMaterial
// Returns:   BulletPhysicsMaterialManager::PhysicsMaterial*
// 
// Returns the physics material from the given material name.
//------------------------------------------------------------------------------
BulletPhysicsMaterialManager::PhysicsMaterial* BulletPhysicsManager::GetMaterial(const std::string& p_strMaterial)
{
	return m_pMaterialManager->GetMaterial(p_strMaterial);
}

//------------------------------------------------------------------------------
// Method:    TickCallback
// Parameter: btDynamicsWorld * p_pWorld
// Parameter: btScalar p_fTimeStep
// Returns:   void
// 
// Callback received for each physics substep. We can do collision checking 
// here in order to send collision events.
//------------------------------------------------------------------------------
void BulletPhysicsManager::TickCallback(btDynamicsWorld *p_pWorld, btScalar p_fTimeStep)
{
	// This is a static callback, so we need to get the BulletPhysicsManager instance from the world user info.
	BulletPhysicsManager* pPhysicsManagerInstance = static_cast<BulletPhysicsManager*>(p_pWorld->getWorldUserInfo());

	// Check for collisions
	int numManifolds = p_pWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; ++i)
	{
		btPersistentManifold* contactManifold = p_pWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const btRigidBody* obA = static_cast<const btRigidBody*>(contactManifold->getBody0());
		const btRigidBody* obB = static_cast<const btRigidBody*>(contactManifold->getBody1());
		const GameObject* pGameObjectA = static_cast<const GameObject*>(obA->getUserPointer());
		const GameObject* pGameObjectB = static_cast<const GameObject*>(obB->getUserPointer());

		int numContacts = contactManifold->getNumContacts();
		for (int j = 0; j < numContacts; ++j)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (pt.getDistance() < 0.0f)
			{
				const btVector3& ptA = pt.getPositionWorldOnA();
				const btVector3& ptB = pt.getPositionWorldOnB();
				const btVector3& normalOnB = pt.m_normalWorldOnB;

				// TODO: do something with the collision here; perhaps store a list of contact pairs in order to generate and queue an event?
			}
		}
	}
}