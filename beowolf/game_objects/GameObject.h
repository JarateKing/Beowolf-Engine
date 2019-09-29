//------------------------------------------------------------------------
// GameObject
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class defines a "GameObject". An GameObject for our purpose is 
// essentially just an empty container to define any object in our game 
// world. The definition of the object is through the aggregation of various
// components.
//------------------------------------------------------------------------

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "ComponentBase.h"
#include "GameObjectManager.h"
#include "Transform.h"

#if defined(_WIN32)
#include "LuaScriptManager.h"
#endif

#include <map>

namespace Common
{
    class GameObjectManager;
    
	class GameObject
	{
		// Typedef for convenience
		typedef std::map<std::string, ComponentBase*> ComponentMap;

		// Only GameObjectManager can create instances (private constructor/destructor)
		friend class GameObjectManager;

	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		const std::string& GetGUID()	{ return m_strGUID; }
		Transform& GetTransform()		{ return m_Transform; }
		GameObjectManager* GetManager() { return m_pGameObjectManager; }

		bool AddComponent(ComponentBase* p_pComponent);
		ComponentBase* GetComponent(const std::string &p_strFamilyId);
		ComponentBase* RemoveComponent(const std::string &p_strFamilyId);
		void DeleteAllComponents();
		void ScheduleDelete();
		bool isScheduledForDelete();

		virtual void Update(float p_fDelta);

        // Lua bindings
#if defined(_WIN32)
		static void ExportToLua();
		bool AddComponentLua(LuaPlus::LuaObject luaObject);
		LuaPlus::LuaObject GetTransformLua();
#endif
		
	private:
		//------------------------------------------------------------------------------
		// Private methods.
		//------------------------------------------------------------------------------

		// Only GameObjectManager can create instances (private constructor/destructor)
		GameObject(GameObjectManager* p_pGameObjectManager);
		~GameObject();

		// Only GameObjectManager can set new GUID as it maintains the mapping of GUID -> GO
		void SetGUID(const std::string& p_strGUID) { m_strGUID = p_strGUID; }

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------

		// A pointer back to the GameObjectManager that created us
		GameObjectManager* m_pGameObjectManager;

		// Running tally for all game objects created
		static unsigned int s_uiGameObjectCount;

		// GUID
		std::string m_strGUID;

		// All entities have a transform
		Transform m_Transform;

		// List of components
		ComponentMap m_mComponentMap;

		// schedule delete
		bool m_shouldDelete;
	};
}

#endif // GAMEOBJECT_H