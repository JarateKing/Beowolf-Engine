//-----------------------------------------------------------------------------
// File:			W_Model.cpp
// Original Author:	Gordon Wood
//
// See header for notes
//-----------------------------------------------------------------------------
#include "W_Model.h"
#include "W_Common.h"
#include "W_BufferManager.h"
#include "W_MaterialManager.h"

namespace wolf
{
static ComponentType gs_aPODTypeMap[] = 
{
    wolf::CT_Invalid,   //EPODDataNone,
	wolf::CT_Float,		//EPODDataFloat,
	wolf::CT_Int,		//EPODDataInt,
	wolf::CT_UShort,	//EPODDataUnsignedShort,
	wolf::CT_UByte4,	//EPODDataRGBA,
	wolf::CT_UByte4,	//EPODDataARGB,
	wolf::CT_UByte4,	//EPODDataD3DCOLOR,
	wolf::CT_UByte4,	//EPODDataUBYTE4,
	wolf::CT_Invalid,	//EPODDataDEC3N,
	wolf::CT_Invalid,	//EPODDataFixed16_16,
	wolf::CT_UByte,		//EPODDataUnsignedByte,
	wolf::CT_Short,		//EPODDataShort,
	wolf::CT_ShortNorm,	//EPODDataShortNorm,
	wolf::CT_Byte,		//EPODDataByte,
	wolf::CT_ByteNorm,	//EPODDataByteNorm,
	wolf::CT_UByteNorm, //EPODDataUnsignedByteNorm,
	wolf::CT_UShortNorm, //EPODDataUnsignedShortNorm,
	wolf::CT_UInt,		//EPODDataUnsignedInt
};

//----------------------------------------------------------
// Constructor
//----------------------------------------------------------
Model::Model(const std::string& p_strFile, const std::string& p_strTexturePrefix) 
{
	// Load in the POD file
	m_pod.ReadFromFile(p_strFile.c_str());

	// Build all the meshes
	for(int i = 0; i < m_pod.nNumMesh; i++)
	{
		SPODMesh* pMesh = &m_pod.pMesh[i];

		// Create the vertex declaration
		wolf::VertexDeclaration* pDecl = new wolf::VertexDeclaration;
		pDecl->Begin();

        // Create the vertex buffer
		wolf::VertexBuffer* pVB = wolf::BufferManager::CreateVertexBuffer(pMesh->pInterleaved, pMesh->nNumVertex * pMesh->sVertex.nStride);
        
		// Create the index buffer
		wolf::IndexBuffer* pIB = wolf::BufferManager::CreateIndexBuffer(pMesh->nNumFaces*3);
		pIB->Write(pMesh->sFaces.pData);
        
		// We'll always have a position
		pDecl->AppendAttribute(wolf::AT_Position, pMesh->sVertex.n, gs_aPODTypeMap[pMesh->sVertex.eType], *((int*)&pMesh->sVertex.pData));

		if( pMesh->sNormals.n > 0 )
			pDecl->AppendAttribute(wolf::AT_Normal, pMesh->sNormals.n, gs_aPODTypeMap[pMesh->sNormals.eType], *((int*)&pMesh->sNormals.pData));

		for( int x = 0; x < pMesh->nNumUVW; x++ )
			pDecl->AppendAttribute((wolf::Attribute)(wolf::AT_TexCoord1 + x), pMesh->psUVW[x].n, gs_aPODTypeMap[pMesh->psUVW[x].eType], *((int*)&pMesh->psUVW[x].pData));

		if( pMesh->sVtxColours.n > 0 )
			pDecl->AppendAttribute(wolf::AT_Color, pMesh->sVtxColours.n, gs_aPODTypeMap[pMesh->sVtxColours.eType], *((int*)&pMesh->sVtxColours.pData));

		pDecl->SetVertexBuffer(pVB);
		pDecl->SetIndexBuffer(pIB);
		pDecl->End();
    
		// Add them to our list of meshes
		Mesh m;
		m.m_pVB = pVB;
		m.m_pIB = pIB;
		m.m_pDecl = pDecl;
		m_lMeshes.push_back(m);
	}

	// Now make sure the material manager knows about all materials we'll need
	// CLASS NOTE: This does a bunch of work more than once (like if many nodes
	// reference the same material - which is likely). This is a candidate for
	// cleanup as an exercise
	for(int i = 0; i < m_pod.nNumMeshNode; i++)
	{
        SPODNode* pNode = &m_pod.pNode[i];
		SPODMaterial* pPODMat = &m_pod.pMaterial[pNode->nIdxMaterial];
        
        // Not been created yet, so add it
		wolf::Material* pMat = wolf::MaterialManager::CreateMaterial(pPODMat->pszName);

		// CLASS NOTE: Provide a way to override shaders in some way? Have a think
		// about this.
		pMat->SetProgram("data/week9/textured.vsh", "data/week9/textured.fsh");

		// Grab the texture it's using and change the filename it expects to
		// be our TGA converted files, in the right path
        if( pPODMat->nIdxTexDiffuse != -1 )
        {
            SPODTexture* pPODTex = &m_pod.pTexture[pPODMat->nIdxTexDiffuse];
                
            std::string strFilename = pPODTex->pszName;
            if( strFilename.find(".jpg") != std::string::npos )
                strFilename = strFilename.substr(0,strFilename.find(".jpg")) + std::string(".tga");
            else if( strFilename.find(".png") != std::string::npos )
                strFilename = strFilename.substr(0,strFilename.find(".png")) + std::string(".tga");

            strFilename = p_strTexturePrefix + strFilename;

            // Create the texture, make sure it's set to repeat, and assign it
            // to the material
            wolf::Texture* pTex = wolf::TextureManager::CreateTexture(strFilename.c_str());
            pTex->SetWrapMode(wolf::Texture::WM_Repeat);
            pMat->SetTexture("texture",pTex);
        }
        
		// CLASS NOTE: I'm not reading blend modes from the materials in the POD
		// file. This is bad and will lead to incorrect rendering. Add some code
		// to look in the POD file for this data and set it up in the
		// wolf::Material accordingly!
	}
}

//----------------------------------------------------------
// Destructor
//----------------------------------------------------------
Model::~Model()
{
	for(unsigned int i = 0; i < m_lMeshes.size(); i++)
	{
		const Mesh& m = m_lMeshes[i];
		wolf::BufferManager::DestroyBuffer(m.m_pVB);
		wolf::BufferManager::DestroyBuffer(m.m_pIB);
		delete m.m_pDecl;
	}
}

//----------------------------------------------------------
// Renders this model
//----------------------------------------------------------
void Model::Render(const glm::mat4& p_mWorld, const glm::mat4& p_mView, const glm::mat4& p_mProj)
{
	// Go through every mesh node in the pod file
	for(int i = 0; i < m_pod.nNumMeshNode; i++)
	{
        SPODNode* pNode = &m_pod.pNode[i];
        
		// Safeguard
        if( pNode->nIdx == -1 )
            continue;
        
		// Get the name of the material this node uses
		const char* strMat = m_pod.pMaterial[pNode->nIdxMaterial].pszName;

		// Grab the material from our map of materials. It should exist now.
		wolf::Material* pMat = wolf::MaterialManager::CreateMaterial(strMat);
		pMat->SetUniform("projection", p_mProj);
		pMat->SetUniform("view", p_mView);

		// Calculate its world matrix from the POD file
		glm::mat4 mWorld;
        m_pod.GetWorldMatrix(*((PVRTMATRIX*)glm::value_ptr(mWorld)), *pNode);
        
        mWorld = p_mWorld * mWorld;
		pMat->SetUniform("world", mWorld);  

		// Apply the material
		pMat->Apply();

		// Bind the source data
		m_lMeshes[pNode->nIdx].m_pDecl->Bind();

		// Draw it!
        glDrawElements(GL_TRIANGLES, m_lMeshes[pNode->nIdx].m_pIB->GetNumIndices(), GL_UNSIGNED_SHORT, 0);
	}
}

}



