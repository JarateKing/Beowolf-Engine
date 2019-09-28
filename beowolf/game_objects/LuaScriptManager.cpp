//------------------------------------------------------------------------
// LuaScriptManager
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// Manager class that helps interact with lua scripts.
//------------------------------------------------------------------------

#include "LuaScriptManager.h"

using namespace Common;

// Static singleton instance
LuaScriptManager* LuaScriptManager::s_pLuaScriptManager = NULL;

//------------------------------------------------------------------------------
// Method:    CreateInstance
// Returns:   void
// 
// Creates the singleton instance.
//------------------------------------------------------------------------------
void LuaScriptManager::CreateInstance()
{
	assert(s_pLuaScriptManager == NULL);
	s_pLuaScriptManager = new LuaScriptManager();
}

//------------------------------------------------------------------------------
// Method:    DestroyInstance
// Returns:   void
// 
// Destroys the singleton instance.
//------------------------------------------------------------------------------
void LuaScriptManager::DestroyInstance()
{
	assert(s_pLuaScriptManager != NULL);
	delete s_pLuaScriptManager;
	s_pLuaScriptManager = NULL;
}

//------------------------------------------------------------------------------
// Method:    Instance
// Returns:   SceneManager::SceneManager*
// 
// Access to singleton instance.
//------------------------------------------------------------------------------
LuaScriptManager* LuaScriptManager::Instance()
{
	assert(s_pLuaScriptManager);
	return s_pLuaScriptManager;
}


//------------------------------------------------------------------------------
// Method:    LuaScriptManager
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
LuaScriptManager::LuaScriptManager()
	:
	m_pLuaState(NULL)
{
	// Initialize LuaPlus
	m_pLuaState = LuaPlus::LuaState::Create(true);
}

//------------------------------------------------------------------------------
// Method:    ~LuaScriptManager
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
LuaScriptManager::~LuaScriptManager()
{
	// Destroy the LuaPlus state
	LuaPlus::LuaState::Destroy(m_pLuaState);
	m_pLuaState = NULL;
}