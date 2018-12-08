//-----------------------------------------------------------------------------
// File:			W_Common.h
// Original Author:	Gordon Wood
//
// Common functions used in all samples
//-----------------------------------------------------------------------------
#ifndef COMMON_H
#define COMMON_H

#include "W_Types.h"
#include <string>

#ifdef DEBUG
#define GL_CHECK_ERROR() { GLenum e = glGetError(); if( e != GL_NO_ERROR ) { printf("GL ERROR: %x\n", e); } }
#else
#define GL_CHECK_ERROR() 
#endif

typedef struct Vertex Vertex;
struct Vertex
{
	GLfloat x, y, z;
	GLubyte r, g, b, a;
	GLfloat u, v;
	GLfloat normalDirX = 0.0f;
	GLfloat normalDirY = 1.0f;
	GLfloat normalDirZ = 0.0f;

	void setNormalDir(glm::vec3 input)
	{
		normalDirX = input.x;
		normalDirY = input.y;
		normalDirZ = input.z;
	}

	glm::vec3 xyz()
	{
		return glm::vec3(x, y, z);
	}

	glm::vec2 xy()
	{
		return glm::vec2(x, y);
	}

	glm::vec2 xz()
	{
		return glm::vec2(x, z);
	}

	glm::vec2 yz()
	{
		return glm::vec2(y, z);
	}
};

enum Pos
{
	North,
	Northeast,
	East,
	Southeast,
	South,
	Southwest,
	West,
	Northwest
};

namespace wolf
{
// Loads in a whole file and returns the contents. User is responsible for then
// delete[]-ing the data. Returns 0 if file not able to be loaded.
void* LoadWholeFile(const std::string& p_strFile);

// Loads in a vertex and pixel shader and combines them into a program
GLuint LoadShaders(const std::string& p_strVSFile, const std::string& p_strPSFile);

bool LoadTGA(const std::string& p_strFile, unsigned int* p_pWidth, unsigned int* p_pHeight, unsigned char** p_ppData);
GLuint CreateTextureFromDDS(const std::string& p_strFile, unsigned int* p_pWidth = 0, unsigned int* p_pHeight = 0, bool* p_pHasMips = 0);
}

#endif
