//-----------------------------------------------------------------------------
// File:			W_Material.cpp
// Original Author:	Gordon Wood
//
// See header for notes
//-----------------------------------------------------------------------------
#include "W_Material.h"
#include "W_Common.h"

namespace wolf
{

bool			Material::m_bDepthTestCurr = false;
bool			Material::m_bDepthWriteCurr = true;
DepthFunc		Material::m_eDepthFuncCurr = DF_Less;
bool			Material::m_bBlendCurr = false;
BlendEquation	Material::m_eBlendEquationCurr = BE_Add;
BlendMode		Material::m_eSrcFactorCurr = BM_One;
BlendMode		Material::m_eDstFactorCurr = BM_Zero;

static GLenum gs_aDepthFuncMap[DF_NUM_DEPTH_FUNCS] =
{
	GL_NEVER,			//DF_Never,
	GL_LESS,			//DF_Less,
	GL_LEQUAL,			//DF_LessEqual,
	GL_EQUAL,			//DF_Equal,
	GL_GREATER,			//DF_Greater,
	GL_GEQUAL,			//DF_GreaterEqual,
	GL_NOTEQUAL,		//DF_NotEqual,
	GL_ALWAYS,			//DF_Always,
};

static GLenum gs_aBlendEquationMap[BE_NUM_BLEND_EQUATIONS] =
{
	GL_FUNC_ADD,				//BE_Add
	GL_FUNC_SUBTRACT,			//BE_Subtract
	GL_FUNC_REVERSE_SUBTRACT	//BE_ReverseSubtract
};

static GLenum gs_aBlendModeMap[] =
{
	GL_SRC_ALPHA,				//BM_SrcAlpha,		
	GL_ONE,						//BM_One,				
	GL_SRC_COLOR,				//BM_SrcColor,
	GL_ONE_MINUS_SRC_COLOR,		//BM_OneMinusSrcColor,	
	GL_ONE_MINUS_SRC_ALPHA,		//BM_OneMinusSrcAlpha,
	GL_DST_ALPHA,				//BM_DstAlpha,			
	GL_ONE_MINUS_DST_ALPHA,		//BM_OneMinusDstAlpha,
	GL_DST_COLOR,				//BM_DstColor,			
	GL_ONE_MINUS_DST_COLOR,		//BM_OneMinusDstColor,	
	GL_ZERO,					//BM_Zero,				
};

//----------------------------------------------------------
// Constructor
//----------------------------------------------------------
Material::Material(const std::string& p_strName) 
	: m_strName(p_strName), m_pProgram(0),
	  m_bDepthTest(true), m_bDepthWrite(true), m_eDepthFunc(DF_LessEqual),
	  m_bBlend(false), m_eSrcFactor(BM_One), m_eDstFactor(BM_Zero), m_eBlendEquation(BE_Add)
{
}

//----------------------------------------------------------
// Destructor
//----------------------------------------------------------
Material::~Material()
{
}

//----------------------------------------------------------
// Sets the whole pipeline up for this material
//----------------------------------------------------------
void Material::Apply()
{
	// Bind the shader
	m_pProgram->Bind();

	// Set all the uniforms we have
	int iTexUnit = 0;

	std::map<std::string, Uniform*>::iterator iter;
	for( iter = m_uniforms.begin(); iter != m_uniforms.end(); iter++ )
	{
		// If it's a texture, we need to do a bit more work as we need to keep
		// track of how many texture units we need and switch to them, and then
		// associate that unit number with the uniform
		if( iter->second->m_eType == UT_Texture )
		{
			TextureUniform* pTexUni = static_cast<TextureUniform*>(iter->second);

			glActiveTexture(GL_TEXTURE0 + iTexUnit);
			pTexUni->m_pValue->Bind();
			m_pProgram->SetUniform(iter->first.c_str(), iTexUnit);
			iTexUnit++;
		}
		else
		{
			iter->second->UploadValue(m_pProgram);
		}
	}

	// Set up depth test
	{
		if( m_bDepthTest != m_bDepthTestCurr )
		{
			m_bDepthTestCurr = m_bDepthTest;

			if( m_bDepthTest )
				glEnable(GL_DEPTH_TEST);
			else
				glDisable(GL_DEPTH_TEST);
		}

		if( m_bDepthTest )
		{
			if( m_eDepthFunc != m_eDepthFuncCurr )
			{
				m_eDepthFuncCurr = m_eDepthFunc;

				glDepthFunc(gs_aDepthFuncMap[m_eDepthFunc]);
			}

			if( m_bDepthWrite != m_bDepthWriteCurr )
			{
				m_bDepthWriteCurr = m_bDepthWrite;

				glDepthMask( m_bDepthWrite );
			}
		}
	}

	// Set up blending
	{
		if( m_bBlend != m_bBlendCurr )
		{
			m_bBlendCurr = m_bBlend;

			if( m_bBlend )
				glEnable(GL_BLEND);
			else
				glDisable(GL_BLEND);
        }
        
        if( m_bBlend )
        {
			if( m_eBlendEquation != m_eBlendEquationCurr )
			{
				m_eBlendEquationCurr = m_eBlendEquation;
				glBlendEquation(gs_aBlendEquationMap[m_eBlendEquation]);
			}

			if( m_eSrcFactor != m_eSrcFactorCurr || m_eDstFactor != m_eDstFactorCurr )
			{
				m_eSrcFactorCurr = m_eSrcFactor;
				m_eDstFactorCurr = m_eDstFactor;
				glBlendFunc(gs_aBlendModeMap[m_eSrcFactor], gs_aBlendModeMap[m_eDstFactor]);
			}
		}
	}
}

//----------------------------------------------------------
// Sets what program this material uses
//----------------------------------------------------------
void Material::SetProgram(const std::string& p_strVS, const std::string& p_strPS)
{
	if( m_pProgram )
		ProgramManager::DestroyProgram(m_pProgram);

	m_pProgram = ProgramManager::CreateProgram(p_strVS, p_strPS);
}

//----------------------------------------------------------
// Set uniform version for mat4 uniforms
//----------------------------------------------------------
void Material::SetUniform(const std::string& p_strName, const glm::mat4& p_m)
{
	std::map<std::string, Uniform*>::iterator iter = m_uniforms.find(p_strName);

	if( iter == m_uniforms.end() )
	{
		m_uniforms.insert( std::pair<std::string, Uniform*>(p_strName, new Matrix4Uniform(p_strName,p_m)));
		return;
	}

	assert(iter->second->m_eType == UT_Matrix4);
	Matrix4Uniform* pUniform = static_cast<Matrix4Uniform*>(iter->second);
	pUniform->m_value = p_m;
}

//----------------------------------------------------------
// Set uniform version for mat3 uniforms
//----------------------------------------------------------
void Material::SetUniform(const std::string& p_strName, const glm::mat3& p_m)
{
	std::map<std::string, Uniform*>::iterator iter = m_uniforms.find(p_strName);

	if( iter == m_uniforms.end() )
	{
		m_uniforms.insert( std::pair<std::string, Uniform*>(p_strName, new Matrix3Uniform(p_strName,p_m)));
		return;
	}

	assert(iter->second->m_eType == UT_Matrix3);
	Matrix3Uniform* pUniform = static_cast<Matrix3Uniform*>(iter->second);
	pUniform->m_value = p_m;
}

//----------------------------------------------------------
// Set uniform version for vec4 uniforms
//----------------------------------------------------------
void Material::SetUniform(const std::string& p_strName, const glm::vec4& p_v)
{
	std::map<std::string, Uniform*>::iterator iter = m_uniforms.find(p_strName);

	if( iter == m_uniforms.end() )
	{
		m_uniforms.insert( std::pair<std::string, Uniform*>(p_strName, new Vector4Uniform(p_strName,p_v)));
		return;
	}

	assert(iter->second->m_eType == UT_Vector4);
	Vector4Uniform* pUniform = static_cast<Vector4Uniform*>(iter->second);
	pUniform->m_value = p_v;
}

//----------------------------------------------------------
// Set uniform version for vec4 uniforms
//----------------------------------------------------------
void Material::SetUniform(const std::string& p_strName, const glm::vec3& p_v)
{
	std::map<std::string, Uniform*>::iterator iter = m_uniforms.find(p_strName);

	if( iter == m_uniforms.end() )
	{
		m_uniforms.insert( std::pair<std::string, Uniform*>(p_strName, new Vector3Uniform(p_strName,p_v)));
		return;
	}

	assert(iter->second->m_eType == UT_Vector3);
	Vector3Uniform* pUniform = static_cast<Vector3Uniform*>(iter->second);
	pUniform->m_value = p_v;
}

//----------------------------------------------------------
// Set uniform version for vec4 uniforms
//----------------------------------------------------------
void Material::SetUniform(const std::string& p_strName, const wolf::Color4& p_c)
{
	std::map<std::string, Uniform*>::iterator iter = m_uniforms.find(p_strName);

	if( iter == m_uniforms.end() )
	{
		m_uniforms.insert( std::pair<std::string, Uniform*>(p_strName, new Color4Uniform(p_strName,p_c)));
		return;
	}

	assert(iter->second->m_eType == UT_Color4);
	Color4Uniform* pUniform = static_cast<Color4Uniform*>(iter->second);
	pUniform->m_value = p_c;
}

//----------------------------------------------------------
// Set uniform version for float uniforms
//----------------------------------------------------------
void Material::SetUniform(const std::string& p_strName, float p_f)
{
	std::map<std::string, Uniform*>::iterator iter = m_uniforms.find(p_strName);

	if( iter == m_uniforms.end() )
	{
		m_uniforms.insert( std::pair<std::string, Uniform*>(p_strName, new FloatUniform(p_strName,p_f)));
		return;
	}

	assert(iter->second->m_eType == UT_Float);
	FloatUniform* pUniform = static_cast<FloatUniform*>(iter->second);
	pUniform->m_value = p_f;
}

//----------------------------------------------------------
// Used to set textures on the material, and associate them
// with names in the shader
//----------------------------------------------------------
void Material::SetTexture(const std::string& p_strName, const Texture* p_pTex)
{
	std::map<std::string, Uniform*>::iterator iter = m_uniforms.find(p_strName);

	if( iter == m_uniforms.end() )
	{
		m_uniforms.insert( std::pair<std::string, Uniform*>(p_strName, new TextureUniform(p_strName,p_pTex)));
		return;
	}

	assert(iter->second->m_eType == UT_Texture);
	TextureUniform* pUniform = static_cast<TextureUniform*>(iter->second);
	pUniform->m_pValue = p_pTex;
}

}


