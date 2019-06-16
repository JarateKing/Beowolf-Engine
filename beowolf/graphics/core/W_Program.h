//-----------------------------------------------------------------------------
// File:			W_Program.h
// Original Author:	Gordon Wood
//
// Class encapsulating a vertex shader and pixel shader combo, and allowing
// easy setting of uniforms on the program
//-----------------------------------------------------------------------------
#ifndef W_PROGRAM_H
#define W_PROGRAM_H

#include "W_Types.h"
#include <string>

namespace wolf
{
class Program
{
	friend class ProgramManager;
	public:
		//-------------------------------------------------------------------------
		// PUBLIC INTERFACE
		//-------------------------------------------------------------------------
		void Bind();

        void SetUniform(const char* p_strName, const glm::mat4& p_m);
        void SetUniform(const char* p_strName, const glm::mat3& p_m);
        void SetUniform(const char* p_strName, const glm::vec3& p_v);
        void SetUniform(const char* p_strName, const glm::vec4& p_v);
		void SetUniform(const char* p_strName, const wolf::Color4& p_c);
		void SetUniform(const char* p_strName, int p_i);
		void SetUniform(const char* p_strName, float p_f);
		//-------------------------------------------------------------------------

	private:
		//-------------------------------------------------------------------------
		// PRIVATE METHODS
		//-------------------------------------------------------------------------

		// Made private to enforce usage via ProgramManager
		Program(const std::string& p_strVS, const std::string& p_strPS);
		virtual ~Program();
		
		bool CompileShader(GLuint* p_pShader, GLenum p_eType, const std::string& p_strFile);
		bool LinkProgram();
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// PRIVATE MEMBERS
		//-------------------------------------------------------------------------
		GLuint		m_uiProgram;
		//-------------------------------------------------------------------------
};

}

#endif
