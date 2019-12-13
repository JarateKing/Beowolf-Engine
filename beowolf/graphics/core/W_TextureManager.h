//-----------------------------------------------------------------------------
// File:			W_TextureManager.h
// Original Author:	Gordon Wood
//
// Class to manage textures. Handles creation of them and dealing with duplicate
// requests for the same texture. Also provides a place for a central means 
// to reload all textures when a context is lost (in a potential future update)
//-----------------------------------------------------------------------------
#ifndef W_TEXTURE_MANAGER_H
#define W_TEXTURE_MANAGER_H

#include "W_Types.h"
#include "W_Texture.h"
#include <string>
#include <map>

namespace wolf
{
class TextureManager
{
	public:
		//-------------------------------------------------------------------------
		// PUBLIC INTERFACE
		//-------------------------------------------------------------------------
		static Texture* CreateTexture(const std::string& p_strFile, bool includeMipmaps = true);
		static Texture* CreateTexture(void* p_pData, unsigned int p_uiWidth, unsigned int p_uiHeight, Texture::Format p_eFormat, bool includeMipmaps = true);
		static void DestroyTexture(Texture* p_pTex);

		// TODO: You should really have a method like "Cleanup" that will delete
		// any leftover textures that weren't destroyed by the game, as
		// a safeguard - or at least prints a warning.
		//-------------------------------------------------------------------------

	private:
		//-------------------------------------------------------------------------
		// PRIVATE TYPES
		//-------------------------------------------------------------------------
		struct Entry
		{
			Texture*	m_pTex;
			int			m_iRefCount;
			Entry(Texture* p_pTex) : m_pTex(p_pTex), m_iRefCount(0) {}
		};
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// PRIVATE MEMBERS
		//-------------------------------------------------------------------------
		static std::map<std::string, Entry*>	m_textures;
		//-------------------------------------------------------------------------
};

}

#endif


