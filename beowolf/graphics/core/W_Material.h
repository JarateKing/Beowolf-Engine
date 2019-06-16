//-----------------------------------------------------------------------------
// File:			W_Material.h
// Original Author:	Gordon Wood
//
// Class representing a material, which is a combination of state + shaders
// + uniforms
//-----------------------------------------------------------------------------
#ifndef W_MATERIAL_H
#define W_MATERIAL_H

#include "W_Types.h"
#include "W_TextureManager.h"
#include "W_ProgramManager.h"
#include <string>
#include <map>

namespace wolf
{
class Material
{
	friend class MaterialManager;
	public:
		//-------------------------------------------------------------------------
		// PUBLIC INTERFACE
		//-------------------------------------------------------------------------
		void Apply();

		void SetUniform(const std::string& p_strName, const glm::mat4& p_m);
		void SetUniform(const std::string& p_strName, const glm::mat3& p_m);
		void SetUniform(const std::string& p_strName, const glm::vec4& p_m);
		void SetUniform(const std::string& p_strName, const glm::vec3& p_m);
		void SetUniform(const std::string& p_strName, const wolf::Color4& p_c);
		void SetUniform(const std::string& p_strName, float p_f);

		void SetTexture(const std::string& p_strName, const Texture* p_pTex);

		void SetDepthTest(bool p_b) { m_bDepthTest = p_b; }
		void SetDepthWrite(bool p_b) { m_bDepthWrite = p_b; }
		void SetDepthFunc(DepthFunc p_eFunc) { m_eDepthFunc = p_eFunc; }

		void SetBlend(bool p_b) { m_bBlend = p_b; }
		void SetBlendMode(BlendMode p_eSrc, BlendMode p_eDst) { m_eSrcFactor = p_eSrc; m_eDstFactor = p_eDst; }
		void SetBlendEquation(BlendEquation p_eEqn) { m_eBlendEquation = p_eEqn; }

		const std::string& GetName() const { return m_strName; }
		void SetProgram(const std::string& p_strVS, const std::string& p_strPS);
		//-------------------------------------------------------------------------

	private:
		//-------------------------------------------------------------------------
		// PRIVATE TYPES
		//-------------------------------------------------------------------------
		enum UniformType
		{
			UT_Matrix4,
			UT_Matrix3,
			UT_Vector3,
			UT_Vector4,
			UT_Color4,
			UT_Float,
			UT_Texture
		};

		struct Uniform
		{
			Uniform(const std::string& p_strName, UniformType p_eType) : m_strName(p_strName), m_eType(p_eType) {}
			virtual void UploadValue(Program* p_pProgram) {}

			UniformType		m_eType;
			std::string     m_strName;
		};
		struct Matrix4Uniform : public Uniform
		{
			Matrix4Uniform(const std::string& p_strName, const glm::mat4& p_m) : Uniform(p_strName,UT_Matrix4), m_value(p_m) {}
			void UploadValue(Program* p_pProgram) { p_pProgram->SetUniform(m_strName.c_str(), m_value); }
			glm::mat4		m_value;
		};
		struct Matrix3Uniform : public Uniform
		{
			Matrix3Uniform(const std::string& p_strName, const glm::mat3& p_m) : Uniform(p_strName,UT_Matrix3), m_value(p_m) {}
			void UploadValue(Program* p_pProgram) { p_pProgram->SetUniform(m_strName.c_str(), m_value); }
			glm::mat3		m_value;
		};
		struct Vector3Uniform : public Uniform
		{
			Vector3Uniform(const std::string& p_strName, const glm::vec3& p_v) : Uniform(p_strName,UT_Vector3), m_value(p_v) {}
			void UploadValue(Program* p_pProgram) { p_pProgram->SetUniform(m_strName.c_str(), m_value); }
			glm::vec3		m_value;
		};
		struct Vector4Uniform : public Uniform
		{
			Vector4Uniform(const std::string& p_strName, const glm::vec4& p_v) : Uniform(p_strName,UT_Vector4), m_value(p_v) {}
			void UploadValue(Program* p_pProgram) { p_pProgram->SetUniform(m_strName.c_str(), m_value); }
			glm::vec4		m_value;
		};
		struct Color4Uniform : public Uniform
		{
			Color4Uniform(const std::string& p_strName, const wolf::Color4& p_c) : Uniform(p_strName,UT_Color4), m_value(p_c) {}
			void UploadValue(Program* p_pProgram) { p_pProgram->SetUniform(m_strName.c_str(), m_value); }
			wolf::Color4		m_value;
		};
		struct FloatUniform : public Uniform
		{
			FloatUniform(const std::string& p_strName, float p_f) : Uniform(p_strName,UT_Float), m_value(p_f) {}
			void UploadValue(Program* p_pProgram) { p_pProgram->SetUniform(m_strName.c_str(), m_value); }
			float		m_value;
		};
		struct TextureUniform : public Uniform
		{
			TextureUniform(const std::string& p_strName, const Texture* p_pTex) : Uniform(p_strName,UT_Texture), m_pValue(p_pTex) {}
			void UploadValue(Program* p_pProgram) { /* This is dealt with differently. See "Apply" */}
			const Texture*		m_pValue;
		};
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// PRIVATE METHODS
		//-------------------------------------------------------------------------
		
		// Made private to enforce creation/deletion via MaterialManager
		Material(const std::string& p_strName);
		virtual ~Material();
		
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// PRIVATE MEMBERS
		//-------------------------------------------------------------------------
		std::string							m_strName;
		Program*							m_pProgram;
		std::map<std::string, Uniform*>		m_uniforms;

		bool								m_bDepthTest;
		DepthFunc							m_eDepthFunc;
		bool								m_bDepthWrite;

		bool								m_bBlend;
		BlendMode							m_eSrcFactor;
		BlendMode							m_eDstFactor;
		BlendEquation						m_eBlendEquation;

		// For caching
		static bool							m_bDepthTestCurr;
		static bool							m_bDepthWriteCurr;
		static DepthFunc					m_eDepthFuncCurr;

		static bool							m_bBlendCurr;
		static BlendEquation				m_eBlendEquationCurr;
		static BlendMode					m_eSrcFactorCurr;
		static BlendMode					m_eDstFactorCurr;
		//-------------------------------------------------------------------------
};

}

#endif


