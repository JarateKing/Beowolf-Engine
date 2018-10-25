//-----------------------------------------------------------------------------
// File:			W_Texture.cpp
// Original Author:	Gordon Wood
//
// See header for notes
//-----------------------------------------------------------------------------
#include "W_Texture.h"
#include "W_Common.h"

namespace wolf
{

static GLenum gs_aFormatMap[Texture::FMT_NUM_FORMATS] =
{
	GL_RGB,		//FMT_888,
	GL_RGBA,	//FMT_8888,
	GL_RGB,		//FMT_565,
	GL_RGBA,	//FMT_4444,
	GL_RGBA		//FMT_5551
};

static GLenum gs_aTypeMap[Texture::FMT_NUM_FORMATS] =
{
	GL_UNSIGNED_BYTE,			//FMT_888,
	GL_UNSIGNED_BYTE,			//FMT_8888,
	GL_UNSIGNED_SHORT_5_6_5,	//FMT_565,
	GL_UNSIGNED_SHORT_4_4_4_4,	//FMT_4444,
	GL_UNSIGNED_SHORT_5_5_5_1	//FMT_5551
};

static GLenum gs_aWrapMap[Texture::WM_NUM_WRAP_MODES] =
{
	GL_REPEAT,					//WM_Repeat,
	GL_CLAMP_TO_EDGE,			//WM_Clamp,
	GL_MIRRORED_REPEAT			//WM_MirroredRepeat,
};

//----------------------------------------------------------
// Constructor, taking in filename to load
//----------------------------------------------------------
Texture::Texture(const std::string& p_strFile) 
	: m_eFilterMin(Texture::FM_Invalid), m_eFilterMag(Texture::FM_Invalid),
	  m_eWrapU(Texture::WM_Invalid), m_eWrapV(Texture::WM_Invalid),
	  m_uiWidth(0), m_uiHeight(0), m_uiTex(0)
{
	if( p_strFile.find(".dds") != std::string::npos )
		LoadFromDDS(p_strFile);
	else if( p_strFile.find(".tga") != std::string::npos )
		LoadFromTGA(p_strFile);
	else
	{
		printf("ERROR: No idea how to load this file - %s!", p_strFile.c_str());
	}

	SetWrapMode(WM_Clamp);
}

//----------------------------------------------------------
// Constructor taking in data already in ram
//----------------------------------------------------------
Texture::Texture(void* p_pData, unsigned int p_uiWidth, unsigned int p_uiHeight, Format p_eFormat)
	: m_eFilterMin(Texture::FM_Invalid), m_eFilterMag(Texture::FM_Invalid),
	  m_eWrapU(Texture::WM_Invalid), m_eWrapV(Texture::WM_Invalid),
	  m_uiWidth(0), m_uiHeight(0), m_uiTex(0)
{
	glGenTextures(1,&m_uiTex);
	glBindTexture(GL_TEXTURE_2D, m_uiTex);

	glTexImage2D(GL_TEXTURE_2D, 0, gs_aFormatMap[p_eFormat], p_uiWidth, p_uiHeight, 0, gs_aFormatMap[p_eFormat], gs_aTypeMap[p_eFormat], p_pData);

	m_uiWidth = p_uiWidth;
	m_uiHeight = p_uiHeight;

	SetWrapMode(WM_Clamp);

	glGenerateMipmap(GL_TEXTURE_2D);
	SetFilterMode(FM_TrilinearMipmap, FM_Linear);
}

//----------------------------------------------------------
// Destructor
//----------------------------------------------------------
Texture::~Texture()
{
	glDeleteTextures(1,&m_uiTex);
}

//----------------------------------------------------------
// Binds this texture as the current one (on the currently
// active texture unit)
//----------------------------------------------------------
void Texture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_uiTex);
}

//----------------------------------------------------------
// Builds the texture from the given DDS file, including mipmap
// levels, if found in the DDS
//----------------------------------------------------------
void Texture::LoadFromDDS(const std::string& p_strFile)
{
	bool bHasMips = false;
	m_uiTex = wolf::CreateTextureFromDDS(p_strFile, &m_uiWidth, &m_uiHeight, &bHasMips);

	if( bHasMips )
		m_eFilterMin = FM_TrilinearMipmap;
	else
		m_eFilterMin = FM_Linear;

	m_eFilterMag = FM_Linear;
}

//----------------------------------------------------------
// Builds the texture from the given TGA file. Mipmap levels
// are automatically generated
//----------------------------------------------------------
void Texture::LoadFromTGA(const std::string& p_strFile)
{
	glGenTextures(1,&m_uiTex);
	glBindTexture(GL_TEXTURE_2D, m_uiTex);

	GLFWimage img;
	glfwReadImage( p_strFile.c_str(), &img, 0 );
    glTexImage2D(GL_TEXTURE_2D, 0, img.Format, img.Width, img.Height, 0, img.Format, GL_UNSIGNED_BYTE, img.Data);
    
    m_uiWidth = img.Width;
	m_uiHeight = img.Height;
    
	glfwFreeImage(&img);

	glGenerateMipmap(GL_TEXTURE_2D);
	SetFilterMode(FM_TrilinearMipmap, FM_Linear);
}

//----------------------------------------------------------
// Sets the wrapping mode on U and V to use for this texture when
// it is bound
//----------------------------------------------------------
void Texture::SetWrapMode(WrapMode p_eWrapU, WrapMode p_eWrapV)
{
	if( p_eWrapV == WM_Invalid )
		p_eWrapV = p_eWrapU;

	if( m_eWrapU == p_eWrapU && m_eWrapV == p_eWrapV )
		return; // Nothing to do, move along!

    Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gs_aWrapMap[p_eWrapU]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gs_aWrapMap[p_eWrapV]);
	m_eWrapU = p_eWrapU;
	m_eWrapV = p_eWrapV;
}

//----------------------------------------------------------
// Sets the filtermode (min and mag) to use for this texture when
// it is bound
//----------------------------------------------------------
void Texture::SetFilterMode(FilterMode p_eFilterMin, FilterMode p_eFilterMag)
{
    if( p_eFilterMag == FM_Invalid )
		p_eFilterMag = p_eFilterMin;

	if( m_eFilterMin == p_eFilterMin && m_eFilterMag == p_eFilterMag )
		return;

	switch( p_eFilterMin )
	{
		case FM_Nearest:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			break;
		case FM_Linear:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			break;
		case FM_NearestMipmap:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			break;
		case FM_TrilinearMipmap:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			break;
        default: ;
	}

	switch( p_eFilterMag )
	{
		case FM_Nearest:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		case FM_Linear:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
        default: ;            
	}

	m_eFilterMin = p_eFilterMin;
	m_eFilterMag = p_eFilterMag;
}

}

