//-----------------------------------------------------------------------------
// File:			W_Texture.h
// Original Author:	Gordon Wood
//
// Class encapsulating a texture
//-----------------------------------------------------------------------------
#ifndef W_TEXTURE_H
#define W_TEXTURE_H

#include "W_Types.h"
#include <string>

namespace wolf
{
class Texture
{
	friend class TextureManager;

	public:
		//-------------------------------------------------------------------------
		// PUBLIC TYPES
		//-------------------------------------------------------------------------
		enum Format
		{
			FMT_888,
			FMT_8888,
			FMT_565,
			FMT_4444,
			FMT_5551,
			FMT_NUM_FORMATS
		};

		enum WrapMode
		{
			WM_Repeat,
			WM_Clamp,
			WM_MirroredRepeat,
			WM_NUM_WRAP_MODES,
			WM_Invalid
		};

		enum FilterMode
		{
			FM_Nearest,
			FM_Linear,
			FM_NearestMipmap,
			FM_LinearMipmap,
			FM_TrilinearMipmap,
			FM_NUM_FILTER_MODES,
			FM_Invalid
		};
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// PUBLIC INTERFACE
		//-------------------------------------------------------------------------
		void Bind() const;

		void SetWrapMode(WrapMode p_eWrapU, WrapMode p_eWrapV = WM_Invalid);
		void SetFilterMode(FilterMode p_eFilterMin, FilterMode p_eFilterMag = FM_Invalid);

		unsigned int GetWidth() const { return m_uiWidth; }
		unsigned int GetHeight() const { return m_uiHeight; }
		//-------------------------------------------------------------------------

	private:
		//-------------------------------------------------------------------------
		// PRIVATE METHODS
		//-------------------------------------------------------------------------

		// Made private to enforce creation/destruction via TextureManager
		Texture(const std::string& p_strFile);
		Texture(void* p_pData, unsigned int p_uiWidth, unsigned int p_uiHeight, Format p_eFormat);
		virtual ~Texture();

		void LoadFromDDS(const std::string& p_strFile);
		void LoadFromTGA(const std::string& p_strFile);
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// PRIVATE MEMBERS
		//-------------------------------------------------------------------------
		GLuint			m_uiTex;
		WrapMode		m_eWrapU;
		WrapMode		m_eWrapV;
		FilterMode		m_eFilterMin;
		FilterMode		m_eFilterMag;
		unsigned int	m_uiWidth;
		unsigned int    m_uiHeight;
		//-------------------------------------------------------------------------
};

}

#endif

