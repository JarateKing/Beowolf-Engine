//------------------------------------------------------------------------
// BulletPhysicsMaterialManager
//
// Created:	2013/02/17
// Author:	Carel Boers
//	
// Manages a set of material properties that can be applied to rigid bodies.
//------------------------------------------------------------------------

#ifndef BULLETPHYSICSMATERIALMANAGER_H
#define BULLETPHYSICSMATERIALMANAGER_H

#include "btBulletDynamicsCommon.h"
#include "BulletPhysicsDebugDrawer.h"
#include "W_LineDrawer.h"

#include <map>

namespace Common
{
	class BulletPhysicsMaterialManager
	{
	public:
		//---------------------------------------------------------------------
		// Public types.
		//---------------------------------------------------------------------

		// Physics material properties
		struct PhysicsMaterial
		{
			float restitution;
			float friction;
			PhysicsMaterial(float _restitution, float _friction) : restitution(_restitution), friction(_friction) {}
		};

		typedef std::map<std::string, PhysicsMaterial*> MaterialMap;

	public:
		//---------------------------------------------------------------------
		// Public Methods.
		//---------------------------------------------------------------------

		BulletPhysicsMaterialManager();
		~BulletPhysicsMaterialManager();

		void LoadMaterials(const std::string &p_strMaterialPath);
		PhysicsMaterial* GetMaterial(const std::string &p_strMaterialName);

	private:
		//---------------------------------------------------------------------
		// Private members.
		//---------------------------------------------------------------------

		// Map of physics materials.
		MaterialMap m_mMaterialMap;
	};
}

#endif // BULLETPHYSICSMATERIALMANAGER_H