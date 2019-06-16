//-----------------------------------------------------------------------------
// File:			W_Model.h
// Original Author:	Gordon Wood
//
// Class representing a model that can be loaded from a POD file
//
// ASSUMPTIONS:
//	1) POD files will always be exported as Index Triangle Lists
//	2) No more than 8 UV channels in the POD file
//	3) Model is exported with interleaved data
//-----------------------------------------------------------------------------
#ifndef W_MODEL_H
#define W_MODEL_H

#include "W_Types.h"
#include "W_VertexBuffer.h"
#include "W_IndexBuffer.h"
#include "W_BufferManager.h"
#include "W_VertexDeclaration.h"
#include "W_TextureManager.h"
#include "W_ProgramManager.h"
#include "PVRTModelPOD.h"
#include <string>
#include <map>
#include <vector>

namespace wolf
{
class Model
{
	public:
		//-------------------------------------------------------------------------
		// PUBLIC INTERFACE
		//-------------------------------------------------------------------------
		Model(const std::string& p_strFile, const std::string& p_strTexturePrefix = "");
		~Model();

		void Render(const glm::mat4& p_mWorld, const glm::mat4& p_mView, const glm::mat4& p_mProj);
		//-------------------------------------------------------------------------

	private:
		//-------------------------------------------------------------------------
		// PRIVATE TYPES
		//-------------------------------------------------------------------------
		struct Mesh
		{
			wolf::VertexBuffer* m_pVB;
			wolf::IndexBuffer* m_pIB;
			wolf::VertexDeclaration* m_pDecl;
		};
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// PRIVATE METHODS
		//-------------------------------------------------------------------------
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// PRIVATE MEMBERS
		//-------------------------------------------------------------------------
		CPVRTModelPOD		m_pod;
		std::vector<Mesh>	m_lMeshes;
		//-------------------------------------------------------------------------
};

}

#endif



