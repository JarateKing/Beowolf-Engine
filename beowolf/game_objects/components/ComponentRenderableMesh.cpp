//------------------------------------------------------------------------
// ComponentRenderableMesh
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a component that is renderable.
//------------------------------------------------------------------------

#include "W_Model.h"
#include "ComponentRenderableMesh.h"
#include "GameObject.h"
#include "SceneManager.h"

using namespace week2;

//------------------------------------------------------------------------------
// Method:    ComponentRenderableMesh
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
ComponentRenderableMesh::ComponentRenderableMesh()
	:
	m_pModel(NULL)
{
}

//------------------------------------------------------------------------------
// Method:    ~ComponentRenderableMesh
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
ComponentRenderableMesh::~ComponentRenderableMesh()
{
	// Remove the model from the scene
	Common::SceneManager::Instance()->RemoveModel(m_pModel);

	// Delete the model
	if (m_pModel)
	{
		delete m_pModel;
		m_pModel = NULL;
	}
}

//------------------------------------------------------------------------------
// Method:    Init
// Parameter: const std::string & p_strPath
// Parameter: const std::string & p_strTexturePath
// Parameter: const std::string & p_strVertexProgramPath
// Parameter: const std::string & p_strFragmentProgramPath
// Returns:   void
// 
// Initializes this component by loading the renderable object.
//------------------------------------------------------------------------------
void ComponentRenderableMesh::Init(const std::string& p_strPath, const std::string &p_strTexturePath, const std::string &p_strVertexProgramPath, const std::string &p_strFragmentProgramPath)
{
	m_pModel = new wolf::Model(p_strPath, p_strTexturePath, p_strVertexProgramPath, p_strFragmentProgramPath);
	Common::SceneManager::Instance()->AddModel(m_pModel);
}

//------------------------------------------------------------------------------
// Method:    SyncTransform
// Returns:   void
// 
// TODO: document me.
//------------------------------------------------------------------------------
void ComponentRenderableMesh::SyncTransform()
{
	m_pModel->SetTransform(this->GetGameObject()->GetTransform().GetTransformation());
}