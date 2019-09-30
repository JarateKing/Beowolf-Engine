//------------------------------------------------------------------------
// BulletPhysicsManager
//
// Created:	2013/02/17
// Author:	Carel Boers
//	
// Manager to handle integration and initialization with Bullet.
//------------------------------------------------------------------------

#ifndef BULLETPHYSICSMANAGER_H
#define BULLETPHYSICSMANAGER_H

#include "btBulletDynamicsCommon.h"
#include "BulletPhysicsDebugDrawer.h"
#include "BulletPhysicsMaterialManager.h"
#include "W_LineDrawer.h"

namespace Common
{
	class BulletPhysicsManager
	{
	public:
		//---------------------------------------------------------------------
		// Public Methods.
		//---------------------------------------------------------------------

		static void CreateInstance(const char* p_strMaterialsPath, const char* p_strDebugLinesVertexShader=0, const char* p_strDebugLinesFragShader=0);
		static void DestroyInstance();
		static BulletPhysicsManager* Instance();

		void Update(float p_fDelta);
		void Render(const glm::mat4& p_mProj, const glm::mat4& p_mView);

		void ToggleDebugRendering();

		// Accessors
		btDiscreteDynamicsWorld* GetWorld() { return m_pDynamicsWorld; }
		BulletPhysicsMaterialManager::PhysicsMaterial* GetMaterial(const std::string& p_strMaterial);

		// InternalTick callback
		static void TickCallback(btDynamicsWorld *p_pWorld, btScalar p_fTimeStep);

	private:
		//--------------------------------------------------------------------
		// Private methods.
		//--------------------------------------------------------------------

		// Private construction; we're a singleton
		BulletPhysicsManager(const char* p_strMaterialsPath, const char* p_strDebugLinesVertexShader, const char* p_strDebugLinesFragShader);
		~BulletPhysicsManager();

	private:
		//---------------------------------------------------------------------
		// Private members.
		//---------------------------------------------------------------------

		// Static singleton instance
		static BulletPhysicsManager* s_pPhysicsManagerInstance;

		// Bullet SDK members
		btDefaultCollisionConfiguration* m_pCollisionConfiguration;
		btCollisionDispatcher* m_pDispatcher;
		btBroadphaseInterface* m_pOverlappingPairCache;
		btSequentialImpulseConstraintSolver* m_pSolver;
		btDiscreteDynamicsWorld* m_pDynamicsWorld;

		// Material manager
		BulletPhysicsMaterialManager* m_pMaterialManager;

		// Debug Drawing
		bool m_bRenderDebug;
		wolf::LineDrawer* m_pLineDrawer;
		BulletPhysicsDebugDrawer* m_pDebugDrawer;
	};
}

#endif