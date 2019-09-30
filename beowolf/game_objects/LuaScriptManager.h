//------------------------------------------------------------------------
// LuaScriptManager
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// Manager class that helps interact with lua scripts.
//------------------------------------------------------------------------

#ifndef LUASCRIPTMANAGER_H
#define LUASCRIPTMANAGER_H

#include "../lib/luaPlus/include/LuaPlus.h"

namespace Common
{
	class LuaScriptManager
	{
	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		static void CreateInstance();
		static void DestroyInstance();
		static LuaScriptManager* Instance();

		LuaPlus::LuaState* GetLuaState() { return m_pLuaState; }

	private:
		//------------------------------------------------------------------------------
		// Private methods.
		//------------------------------------------------------------------------------

		// Private; we're a singleton
		LuaScriptManager();
		virtual ~LuaScriptManager();

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------

		// Static singleton instance
		static LuaScriptManager* s_pLuaScriptManager;

		LuaPlus::LuaState* m_pLuaState;
	};
}

#endif // LUASCRIPTMANAGER_H

