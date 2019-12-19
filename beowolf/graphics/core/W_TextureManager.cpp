//-----------------------------------------------------------------------------
// File:			W_TextureManager.cpp
// Original Author:	Gordon Wood
//
// See header for notes
//-----------------------------------------------------------------------------
#include "W_TextureManager.h"

namespace wolf
{

std::map<std::string, TextureManager::Entry*>	TextureManager::m_textures;

//----------------------------------------------------------
// Creates a new texture or returns an existing copy if already
// loaded previously
//----------------------------------------------------------
Texture* TextureManager::CreateTexture(const std::string& p_strFile, bool includeMipmaps)
{
	std::map<std::string, Entry*>::iterator iter = m_textures.find(p_strFile);

	if( iter != m_textures.end() )
	{
		iter->second->m_iRefCount++;
		return iter->second->m_pTex;
	}

	Texture* pTex = new Texture(p_strFile);
	Entry* pEntry = new Entry(pTex);
	m_textures[p_strFile] = pEntry;
	return pTex;
}

//----------------------------------------------------------
// When creating directly from data, we don't check for duplicates
// and just delegate directly to the texture class
//----------------------------------------------------------
Texture* TextureManager::CreateTexture(void* p_pData, unsigned int p_uiWidth, unsigned int p_uiHeight, Texture::Format p_eFormat, bool includeMipmaps)
{
	return new Texture(p_pData, p_uiWidth, p_uiHeight, p_eFormat, includeMipmaps);
}

//----------------------------------------------------------
// Destroys a texture. Only actually deletes it if the refcount
// is down to 0.
//----------------------------------------------------------
void TextureManager::DestroyTexture(Texture* p_pTex)
{
	std::map<std::string, Entry*>::iterator iter;
	for( iter = m_textures.begin(); iter != m_textures.end(); iter++ )
	{
		if( iter->second->m_pTex == p_pTex )
		{
			iter->second->m_iRefCount--;
			if( iter->second->m_iRefCount == 0 )
			{
				delete iter->second->m_pTex;
				m_textures.erase(iter);
			}
			return;
		}
	}

	// If we got here, it can only be via a from-data texture which wasn't in
	// the list (unless the pointer being passed in is dodgy in which case we're
	// in trouble anyway).
	delete p_pTex;
}

}


