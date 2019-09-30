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

// FACTORY
Common::ComponentBase* ComponentRenderableMesh::CreateComponent(TiXmlNode* p_node)
{
	std::string model;
	std::string textures;
	std::string vertexprogram;
	std::string fragmentprogram;

	for (TiXmlElement* i = p_node->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
	{
		std::string name = i->Value();

		if (name == "Model")
		{
			model = i->Attribute("path");
		}
		else if (name == "Textures")
		{
			textures = i->Attribute("path");
		}
		else if (name == "VertexProgram")
		{
			vertexprogram = i->Attribute("path");
		}
		else if (name == "FragmentProgram")
		{
			fragmentprogram = i->Attribute("path");
		}
	}

	if (model.length() == 0 || textures.length() == 0 || vertexprogram.length() == 0 || fragmentprogram.length() == 0)
	{
		return NULL;
	}

	ComponentRenderableMesh* component = new ComponentRenderableMesh();
	component->Init(model, textures, vertexprogram, fragmentprogram);

	return component;
}