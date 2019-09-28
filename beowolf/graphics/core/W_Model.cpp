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
Model::Model(const std::string& p_strFile, const std::string& p_strTexturePrefix, const std::string& p_strVertexProgram, const std::string& p_strFragmentProgram)
	:
	m_fFrame(0.0f),
	m_pCurrentAnim(nullptr)
{
	// Load in the POD file
	EPVRTError error = m_pod.ReadFromFile(p_strFile.c_str());

	// Build all the meshes
	for(unsigned int i = 0; i < m_pod.nNumMesh; i++)
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

		for(unsigned int x = 0; x < pMesh->nNumUVW; x++ )
			pDecl->AppendAttribute((wolf::Attribute)(wolf::AT_TexCoord1 + x), pMesh->psUVW[x].n, gs_aPODTypeMap[pMesh->psUVW[x].eType], *((int*)&pMesh->psUVW[x].pData));

		if( pMesh->sVtxColours.n > 0 )
			pDecl->AppendAttribute(wolf::AT_Color, pMesh->sVtxColours.n, gs_aPODTypeMap[pMesh->sVtxColours.eType], *((int*)&pMesh->sVtxColours.pData));

		if( pMesh->sBoneIdx.n != 0 )
			pDecl->AppendAttribute(wolf::AT_BoneIndices, pMesh->sBoneIdx.n, gs_aPODTypeMap[pMesh->sBoneIdx.eType], *((int*)&pMesh->sBoneIdx.pData));

		if( pMesh->sBoneWeight.n != 0 )
			pDecl->AppendAttribute(wolf::AT_BoneWeight, pMesh->sBoneWeight.n, gs_aPODTypeMap[pMesh->sBoneWeight.eType], *((int*)&pMesh->sBoneWeight.pData));

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
	for(unsigned int i = 0; i < m_pod.nNumMeshNode; i++)
	{
        SPODNode* pNode = &m_pod.pNode[i];

		if (pNode->nIdxMaterial == -1)
		{
			continue;
		}

		SPODMaterial* pPODMat = &m_pod.pMaterial[pNode->nIdxMaterial];
        
		Material* pMaterial = wolf::MaterialManager::CreateMaterial(pPODMat->pszName);

		// CLASS NOTE: Provide a way to override shaders in some way? Have a think
		// about this.
		if (!p_strVertexProgram.empty() && !p_strFragmentProgram.empty())
		{
			pMaterial->SetProgram(p_strVertexProgram, p_strFragmentProgram);
		}
		else
		{
			pMaterial->SetProgram("../resources/shaders/textured.vsh", "../resources/shaders/textured.fsh");
		}

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
			pMaterial->SetTexture("texture1", pTex);
		}
		// Specular map texture if it has one
		if (pPODMat->nIdxTexSpecularColour != -1)
		{
			SPODTexture* pPODTex = &m_pod.pTexture[pPODMat->nIdxTexSpecularColour];
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
			pMaterial->SetTexture("texture2", pTex);
		}

		m_lMaterials.push_back(pMaterial);

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

	for (auto &animInfo : m_animations)
	{
		delete animInfo.second;
	}
	m_animations.clear();
}

//----------------------------------------------------------
// Updates this model
//----------------------------------------------------------
void Model::Update(float p_fDelta)
{
	if (m_pCurrentAnim != nullptr)
	{
		m_fFrame += (p_fDelta * 30.0f);
		if (m_fFrame > m_pCurrentAnim->nNumFrame - 1)
		{
			m_fFrame = 0;
		}
		m_pCurrentAnim->SetFrame(m_fFrame);
	}

}

//----------------------------------------------------------
// Renders this model
//----------------------------------------------------------
void Model::Render(const glm::mat4& p_mView, const glm::mat4& p_mProj)
{
	// Go through every mesh node in the pod file
	for(unsigned int i = 0; i < m_pod.nNumMeshNode; i++)
	{
        SPODNode* pNode = &m_pod.pNode[i];
		SPODMesh* pMesh = &m_pod.pMesh[pNode->nIdx];
		Material* pMaterial = m_lMaterials[(pNode->nIdxMaterial >= 0 && pNode->nIdxMaterial < m_lMaterials.size()) ? pNode->nIdxMaterial : 0];
        
		// Safeguard
        if( pNode->nIdx == -1 )
            continue;

		pMaterial->SetUniform("projection", p_mProj);
		pMaterial->SetUniform("view", p_mView);

		// Calculate its world matrix from the POD file
		glm::mat4 mWorld;
        m_pod.GetWorldMatrix(*((PVRTMATRIX*)glm::value_ptr(mWorld)), *pNode);
        
        mWorld = m_mWorldTransform * mWorld;
		pMaterial->SetUniform("world", mWorld);

		glm::mat4 mWVP = p_mProj * p_mView * mWorld;
		glm::mat3 mWorldIT(mWorld);
		mWorldIT = glm::inverse(mWorldIT);
		mWorldIT = glm::transpose(mWorldIT);

		pMaterial->SetUniform("WorldViewProj", mWVP);
		pMaterial->SetUniform("WorldIT", mWorldIT);


		// Bind the source data
		m_lMeshes[pNode->nIdx].m_pDecl->Bind();

		if (pMesh->sBoneIdx.n > 0)
		{
			for(int i32Batch = 0; i32Batch < pMesh->sBoneBatches.nBatchCnt; ++i32Batch)
			{
				// Go through the bones for the current bone batch
				glm::mat4 amBoneWorld2[9];
				glm::mat3 afBoneWorldIT2[9], mBoneIT2;

				int i32Count = pMesh->sBoneBatches.pnBatchBoneCnt[i32Batch];
				for(int i = 0; i < i32Count; ++i)
				{
					// Get the Node of the bone
					int i32NodeID = pMesh->sBoneBatches.pnBatches[i32Batch * pMesh->sBoneBatches.nBatchBoneMax + i];
					if (m_pCurrentAnim != nullptr)
					{
						m_pCurrentAnim->SetFrame(0);

						PVRTMat4 mTmp;
						PVRTMatrixIdentity(mTmp);
						for (int i32AnimNodeID = 0; i32AnimNodeID < m_pCurrentAnim->nNumNode; ++i32AnimNodeID)
						{
							SPODNode pMeshBoneNode = m_pod.pNode[i32NodeID];
							SPODNode pAnimBoneNode = m_pCurrentAnim->pNode[i32AnimNodeID];
							if (strcmp(pMeshBoneNode.pszName, pAnimBoneNode.pszName) == 0)
							{
								PVRTMat4 mMeshBoneTransform;
								m_pod.GetWorldMatrixNoCache(mMeshBoneTransform, pMeshBoneNode);
								m_pCurrentAnim->GetWorldMatrixNoCache(mTmp, pAnimBoneNode);
								PVRTMatrixInverse(mTmp, mTmp);
								PVRTMatrixInverse(mTmp, mMeshBoneTransform);
								break;
							}
						}
						PVRTMat4 mOut = m_pod.GetWorldMatrix(*pNode);
						PVRTMatrixMultiply(mOut, mOut, mTmp);

						m_pCurrentAnim->SetFrame(m_fFrame);

						// Find the matching bone in the animation skeleton
						for (int i32AnimNodeID = 0; i32AnimNodeID < m_pCurrentAnim->nNumNode; ++i32AnimNodeID)
						{
							SPODNode pMeshNode = m_pod.pNode[i32NodeID];
							SPODNode pAnimNode = m_pCurrentAnim->pNode[i32AnimNodeID];
							if (strcmp(pMeshNode.pszName, pAnimNode.pszName) == 0)
							{
								m_pCurrentAnim->GetWorldMatrixNoCache(mTmp, pAnimNode);
								break;
							}
						}
						PVRTMatrixMultiply(mOut, mOut, mTmp);
						amBoneWorld2[i] = *(glm::mat4*)&mOut;
					}
					else
					{
						PVRTMat4 mOut = m_pod.GetBoneWorldMatrix(*pNode, m_pod.pNode[i32NodeID]);
						amBoneWorld2[i] = *(glm::mat4*)&mOut;
					}

					// Calculate the inverse transpose of the 3x3 rotation/scale part for correct lighting
					glm::mat3 mTemp(amBoneWorld2[i]);
					afBoneWorldIT2[i] = glm::transpose(glm::inverse(mTemp));
				}
				pMaterial->SetUniform("BoneMatrixArray", amBoneWorld2, i32Count);
				pMaterial->SetUniform("BoneMatrixArrayIT", afBoneWorldIT2, i32Count);

				// Find number of triangles to draw for this batch
				int i32Tris;
				if(i32Batch+1 < pMesh->sBoneBatches.nBatchCnt)
					i32Tris = pMesh->sBoneBatches.pnBatchOffset[i32Batch+1] - pMesh->sBoneBatches.pnBatchOffset[i32Batch];
				else
					i32Tris = pMesh->nNumFaces - pMesh->sBoneBatches.pnBatchOffset[i32Batch];

				// Apply the material
				pMaterial->Apply();

				// Draw it!
				size_t offset = sizeof(GLushort) * 3 * pMesh->sBoneBatches.pnBatchOffset[i32Batch];
				glDrawElements(GL_TRIANGLES, i32Tris * 3, GL_UNSIGNED_SHORT, (void*) offset);
			}
		}
		else
		{
			// Apply the material
			pMaterial->Apply();

			// Draw it!
			glDrawElements(GL_TRIANGLES, m_lMeshes[pNode->nIdx].m_pIB->GetNumIndices(), GL_UNSIGNED_SHORT, 0);
		}
	}
}

void Model::AddAnimation(const std::string &p_strName, const std::string& p_strFile)
{
	CPVRTModelPOD* anim = new CPVRTModelPOD();

	EPVRTError error = anim->ReadFromFile(p_strFile.c_str());
	if (error != PVR_SUCCESS)
	{
		delete anim;
		return;
	}

	m_animations.insert(std::make_pair(p_strName, anim));
}

void Model::AddAnimations(const std::map<std::string, std::string> &p_animNameToFileMap)
{
	for (auto &animInfo : p_animNameToFileMap)
	{
		AddAnimation(animInfo.first, animInfo.second);
	}
}

void Model::SetAnim(const std::string &p_strName)
{
	if (m_strCurrentAnim.compare(p_strName) == 0) 
	{
		return;
	}

	auto iter = m_animations.find(p_strName);
	if (iter != m_animations.end())
	{
		m_pCurrentAnim = iter->second;
		m_strCurrentAnim = p_strName;
	}
}

void Model::SetAnimFrame(float p_fFrame)
{
	m_fFrame = p_fFrame;
	if (m_pCurrentAnim != nullptr)
	{
		if (m_fFrame > m_pCurrentAnim->nNumFrame - 1)
		{
			m_fFrame = 0;
		}
		m_pCurrentAnim->SetFrame(m_fFrame);
	}
	else
	{
		if (m_fFrame > m_pod.nNumFrame - 1)
		{
			m_fFrame = 0;
		}
		m_pod.SetFrame(m_fFrame);
	}
}

}



