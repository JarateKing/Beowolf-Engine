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

#include "GameObject.h"
#include <sstream>

using namespace Common;

// Static game object counter
unsigned int GameObject::s_uiGameObjectCount = 0;

//------------------------------------------------------------------------------
// Method:    GameObject
// Parameter: GameObjectManager * p_pGameObjectManager
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
GameObject::GameObject(GameObjectManager* p_pGameObjectManager)
	:
	m_pGameObjectManager(p_pGameObjectManager)
{
	// Increment count of all GOs ever created on this machine
	++s_uiGameObjectCount;

	// Create a unique (per this application instance) guid
	std::ostringstream ss;
	ss << "GameObject_" << s_uiGameObjectCount;
	m_strGUID = ss.str();
}

//------------------------------------------------------------------------------
// Method:    ~GameObject
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
GameObject::~GameObject()
{
	this->DeleteAllComponents();
}

//------------------------------------------------------------------------------
// Method:    AddComponent
// Parameter: BaseComponent * p_pComponent
// Returns:   bool
// 
// Add a component to the GameObject. Returns true if it was added, false 
// otherwise.
//------------------------------------------------------------------------------
bool GameObject::AddComponent(ComponentBase* p_pComponent)
{
	// Make sure we don't already have a component of this type
	if (m_mComponentMap.find(p_pComponent->FamilyID()) == m_mComponentMap.end())
	{
		m_mComponentMap.insert(std::pair<std::string, ComponentBase*>(p_pComponent->FamilyID(), p_pComponent));
		p_pComponent->SetGameObject(this);
		return true;
	}

	// Already have a component with this family Id.
	return false;
}

//------------------------------------------------------------------------------
// Method:    GetComponent
// Parameter: const std::string & p_strFamilyId
// Returns:   BaseComponent*
// 
// Get a component with the given family Id. Returns NULL if it isn't found.
//------------------------------------------------------------------------------
ComponentBase* GameObject::GetComponent(const std::string &p_strFamilyId)
{
	ComponentMap::iterator it = m_mComponentMap.find(p_strFamilyId);
	if (it != m_mComponentMap.end())
	{
		return (ComponentBase*)it->second;
	}

	// Don't have it.
	return NULL;
}

//------------------------------------------------------------------------------
// Method:    RemoveComponent
// Parameter: const std::string & p_strFamilyId
// Returns:   BaseComponent*
// 
// Remove a component with the given family Id and returns it. It is not 
// deleted to allow moving this component to another GameObject if desired. 
//------------------------------------------------------------------------------
ComponentBase* GameObject::RemoveComponent(const std::string &p_strFamilyId)
{
	ComponentBase* pComponent = NULL;

	ComponentMap::iterator it = m_mComponentMap.find(p_strFamilyId);
	if (it != m_mComponentMap.end())
	{
		pComponent = (ComponentBase*)it->second;
		m_mComponentMap.erase(it);
	}

	return pComponent;
}

//------------------------------------------------------------------------------
// Method:    DeleteAllComponents
// Returns:   void
// 
// Remove all components from the GameObject and deletes them.
//------------------------------------------------------------------------------
void GameObject::DeleteAllComponents()
{
	ComponentMap::iterator it = m_mComponentMap.begin(), end = m_mComponentMap.end();
	for (; it != end; ++it)
	{
		ComponentBase* pComponent = (ComponentBase*)it->second;
		delete pComponent;
	}
	m_mComponentMap.clear();
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Updates the internal components of the GameObject.
//------------------------------------------------------------------------------
void GameObject::Update(float p_fDelta)
{
	ComponentMap::iterator it = m_mComponentMap.begin(), end = m_mComponentMap.end();
	for (; it != end; ++it)
	{
		ComponentBase* pComponent = (ComponentBase*)it->second;
		pComponent->Update(p_fDelta);
	}
}

#if defined(_WIN32)

//------------------------------------------------------------------------------
// Method:    ExportToLua
// Returns:   void
// 
// Export interface to Lua.
//------------------------------------------------------------------------------
void GameObject::ExportToLua()
{
	LuaPlus::LuaObject metaTable = LuaScriptManager::Instance()->GetLuaState()->GetGlobals().CreateTable("GameObjectMetaTable");
	metaTable.SetObject("__index", metaTable);

	// Register methods
	metaTable.RegisterObjectDirect("GetTransform", (GameObject*) 0, &GameObject::GetTransformLua);
	metaTable.RegisterObjectDirect("AddComponent", (GameObject*) 0, &GameObject::AddComponentLua);
}

//------------------------------------------------------------------------------
// Method:    AddComponentLua
// Parameter: LuaPlus::LuaObject luaObject
// Returns:   bool
// 
// Add component interface to the given Game Object.
//------------------------------------------------------------------------------
bool GameObject::AddComponentLua(LuaPlus::LuaObject luaObject)
{
	if (luaObject.IsTable())
	{
		LuaPlus::LuaObject obj = luaObject.GetByName("__object");
		if (!obj.IsNil())
		{
			ComponentBase* pComponent = static_cast<ComponentBase*>(obj.GetLightUserData());
			this->AddComponent(pComponent);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

//------------------------------------------------------------------------------
// Method:    GetTransformLua
// Returns:   LuaPlus::LuaObject
// 
// Lua version of GetTransform that returns a LuaObject.
//------------------------------------------------------------------------------
LuaPlus::LuaObject GameObject::GetTransformLua()
{
	LuaPlus::LuaObject luaInstance;
	luaInstance.AssignNewTable(LuaScriptManager::Instance()->GetLuaState());

	luaInstance.SetLightUserData("__object", &m_Transform);

	LuaPlus::LuaObject metaTable = LuaScriptManager::Instance()->GetLuaState()->GetGlobals().GetByName("TransformMetaTable");
	luaInstance.SetMetaTable(metaTable);

	return luaInstance;
}

 #endif