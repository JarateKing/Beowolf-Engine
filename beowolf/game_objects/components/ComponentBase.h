//------------------------------------------------------------------------
// ComponentBase
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class contains the minimum interface that all game components
// must adhere to in order to be added to a GameObject.
//------------------------------------------------------------------------

#ifndef BASECOMPONENT_H
#define BASECOMPONENT_H

#include <string>

#if defined(_WIN32)
#include "LuaScriptManager.h"
#endif

namespace Common
{
    class GameObject;
    
	class ComponentBase
	{
		friend class GameObject;
	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		ComponentBase() : m_pGameObject(NULL) {}
		virtual ~ComponentBase() {}

		virtual const std::string FamilyID() = 0;
		virtual const std::string ComponentID() = 0;
		virtual void Update(float p_fDelta) {}

		GameObject* GetGameObject() { return m_pGameObject; }

		// Lua bindings
#if defined(_WIN32)
		static void ExportToLua()
		{
			LuaPlus::LuaObject metaTable = LuaScriptManager::Instance()->GetLuaState()->GetGlobals().CreateTable("ComponentBaseMetaTable");
			metaTable.SetObject("__index", metaTable);
			metaTable.RegisterObjectDirect("GetGameObject", (ComponentBase*) 0, &ComponentBase::GetGameObject);
		}
#endif

	private:
		//------------------------------------------------------------------------------
		// Private methods.
		//------------------------------------------------------------------------------

		// Set parent GameObject. Private so only GameObjects can do this.
		void SetGameObject(GameObject* p_pGameObject)	{ m_pGameObject = p_pGameObject; }

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------

		// Parent GameObject that owns this component
		GameObject* m_pGameObject;
	};
}

#endif // BASECOMPONENT_H