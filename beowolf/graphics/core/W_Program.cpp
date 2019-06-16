//-----------------------------------------------------------------------------
// File:			W_Program.cpp
// Original Author:	Gordon Wood
//
// See header for notes
//-----------------------------------------------------------------------------
#include "W_Program.h"
#include "W_Common.h"

namespace wolf
{

static const char* gs_aAttributeMap[wolf::AT_NUM_ATTRIBS] =
{
	"a_position",	//AT_Position = 0,
	"a_color",		//AT_Color,
	"a_uv1",		//AT_TexCoord1	
	"a_uv2",		//AT_TexCoord2	
	"a_uv3",		//AT_TexCoord3	
	"a_uv4",		//AT_TexCoord4	
	"a_uv5",		//AT_TexCoord5	
	"a_uv6",		//AT_TexCoord6	
	"a_uv7",		//AT_TexCoord7	
	"a_uv8",		//AT_TexCoord8	
	"a_normal",		//AT_Normal
	"a_tangent",	//AT_Tangent
};

//----------------------------------------------------------
// Constructor
//----------------------------------------------------------
Program::Program(const std::string& p_strVS, const std::string& p_strPS) : m_uiProgram(0)
{
    GLuint uiVS, uiPS;
    
    // 1. Create and compile vertex shader.
    if( !CompileShader(&uiVS, GL_VERTEX_SHADER, p_strVS))
	{
        printf("Failed to compile vertex shader\n");
        return;
    }
    
    // 2. Create and compile fragment shader.
    if( !CompileShader(&uiPS, GL_FRAGMENT_SHADER, p_strPS))
	{
        printf("Failed to compile pixel shader\n");
		glDeleteShader(uiVS);
        return;
    }

    // 3. Create shader program that we'll (hopefully) eventually return
    m_uiProgram = glCreateProgram();
    
    // 4. Attach vertex shader and pixel shader to program.
    glAttachShader(m_uiProgram, uiVS);
    glAttachShader(m_uiProgram, uiPS);
    
    // 5. Bind attribute locations. This needs to be done prior to linking.
	for( int i = 0; i < wolf::AT_NUM_ATTRIBS; i++ )
	{
		glBindAttribLocation(m_uiProgram, i, gs_aAttributeMap[i]);
	}
    
    // 6. Link program.
    if( !LinkProgram() )
    {
        printf("Failed to link program: %d\n", m_uiProgram);
        
        if( uiVS )
        {
            glDeleteShader(uiVS);
        }
        if( uiPS )
        {
            glDeleteShader(uiPS);
        }
        if( m_uiProgram )
        {
            glDeleteProgram(m_uiProgram);
			m_uiProgram = 0;
        }
        return;
    }
    
    // Release vertex and fragment shaders.
    if( uiVS )
        glDeleteShader(uiVS);
    if( uiPS )
        glDeleteShader(uiPS);
}

//----------------------------------------------------------
// Destructor
//----------------------------------------------------------
Program::~Program()
{
	if( m_uiProgram )
		glDeleteProgram(m_uiProgram);
}

//----------------------------------------------------------
// Binds this program as the current one
//----------------------------------------------------------
void Program::Bind()
{
	glUseProgram(m_uiProgram);
}

//----------------------------------------------------------
// Sets a matrix uniform of the given name
//----------------------------------------------------------
void Program::SetUniform(const char* p_strName, const glm::mat4& p_m)
{
	int iLoc = glGetUniformLocation(m_uiProgram,p_strName);
	if( iLoc == -1 )
	{
		//printf("WARNING: Unknown uniform %s\n", p_strName);
		return;
	}
	glUniformMatrix4fv(iLoc, 1, GL_FALSE, glm::value_ptr(p_m));
}

//----------------------------------------------------------
// Sets a matrix uniform of the given name
//----------------------------------------------------------
void Program::SetUniform(const char* p_strName, const glm::mat3& p_m)
{
	int iLoc = glGetUniformLocation(m_uiProgram,p_strName);
	if( iLoc == -1 )
	{
		//printf("WARNING: Unknown uniform %s\n", p_strName);
		return;
	}
	glUniformMatrix3fv(iLoc, 1, GL_FALSE, glm::value_ptr(p_m));
}

//----------------------------------------------------------
// Sets a vector uniform of the given name
//----------------------------------------------------------
void Program::SetUniform(const char* p_strName, const glm::vec3& p_v)
{
	int iLoc = glGetUniformLocation(m_uiProgram,p_strName);
	if( iLoc == -1 )
	{
		//printf("WARNING: Unknown uniform %s\n", p_strName);
		return;
	}
	glUniform3fv(iLoc, 1, glm::value_ptr(p_v));
}

//----------------------------------------------------------
// Sets a vector uniform of the given name
//----------------------------------------------------------
void Program::SetUniform(const char* p_strName, const glm::vec4& p_v)
{
	int iLoc = glGetUniformLocation(m_uiProgram,p_strName);
	if( iLoc == -1 )
	{
		//printf("WARNING: Unknown uniform %s\n", p_strName);
		return;
	}
	glUniform4fv(iLoc, 1, glm::value_ptr(p_v));
}

//----------------------------------------------------------
// Sets a color uniform of the given name
//----------------------------------------------------------
void Program::SetUniform(const char* p_strName, const wolf::Color4& p_c)
{
	int iLoc = glGetUniformLocation(m_uiProgram,p_strName);
	if( iLoc == -1 )
	{
		//printf("WARNING: Unknown uniform %s\n", p_strName);
		return;
	}
	glUniform4fv(iLoc, 1, (GLfloat*)&p_c);
}

//----------------------------------------------------------
// Sets an integer uniform of the given name
//----------------------------------------------------------
void Program::SetUniform(const char* p_strName, int p_i)
{
	int iLoc = glGetUniformLocation(m_uiProgram,p_strName);
	if( iLoc == -1 )
	{
		//printf("WARNING: Unknown uniform %s\n", p_strName);
		return;
	}
	glUniform1i(iLoc, p_i);
}

//----------------------------------------------------------
// Sets a floating point uniform of the given name
//----------------------------------------------------------
void Program::SetUniform(const char* p_strName, float p_f)
{
	int iLoc = glGetUniformLocation(m_uiProgram,p_strName);
	if( iLoc == -1 )
	{
		//printf("WARNING: Unknown uniform %s\n", p_strName);
		return;
	}
	glUniform1f(iLoc, p_f);
}

//----------------------------------------------------------
// Compiles the shader of the given type, returning true
// on success, else false. Shader object stored in p_pShader
// on success
//----------------------------------------------------------
bool Program::CompileShader(GLuint* p_pShader, GLenum p_eType, const std::string& p_strFile)
{
    GLint iStatus;
    const GLchar* pSource;
    
    pSource = (GLchar *) wolf::LoadWholeFile(p_strFile);
    if( !pSource )
    {
        printf("Failed to load vertex shader\n");
        return false;
    }
    
    *p_pShader = glCreateShader(p_eType);
    glShaderSource(*p_pShader, 1, &pSource, NULL);
    glCompileShader(*p_pShader);
    
#if defined(DEBUG)
    GLint iLogLength;
    glGetShaderiv(*p_pShader, GL_INFO_LOG_LENGTH, &iLogLength);
    if( iLogLength > 0 )
    {
        GLchar* pLog = (GLchar *)malloc(iLogLength);
        glGetShaderInfoLog(*p_pShader, iLogLength, &iLogLength, pLog);
        printf("Shader compile log:\n%s\n", pLog);
        free(pLog);
    }
#endif
    
    delete[] pSource;
    
    glGetShaderiv(*p_pShader, GL_COMPILE_STATUS, &iStatus);
    if( iStatus == 0 )
    {
        glDeleteShader(*p_pShader);
        return false;
    }
    
    return true;
}

//----------------------------------------------------------
// Performs the linking stage of the vertex and pixel shader,
// resulting in a final program object to use. Checks for
// errors and prints them and returns true on success, else false
//----------------------------------------------------------
bool Program::LinkProgram()
{
    GLint iStatus;
    
    glLinkProgram(m_uiProgram);
    
#if defined(DEBUG)
    GLint iLogLength;
    glGetProgramiv(m_uiProgram, GL_INFO_LOG_LENGTH, &iLogLength);
    if (iLogLength > 0)
    {
        GLchar* pLog = (GLchar *)malloc(iLogLength);
        glGetProgramInfoLog(m_uiProgram, iLogLength, &iLogLength, pLog);
        printf("Program link log:\n%s\n", pLog);
        free(pLog);
    }
#endif
    
    glGetProgramiv(m_uiProgram, GL_LINK_STATUS, &iStatus);
    if (iStatus == 0)
        return false;
    
    return true;
}


}
