//------------------------------------------------------------------------
// BulletPhysicsMaterialManager
//
// Created:	2013/02/17
// Author:	Carel Boers
//	
// Manages a set of material properties that can be applied to rigid bodies.
//------------------------------------------------------------------------

#include "BulletPhysicsMaterialManager.h"
#include "tinyxml.h"	// For parsing material XML

using namespace Common;

//------------------------------------------------------------------------------
// Method:    BulletPhysicsMaterialManager
// Parameter: void
// Returns:   
// 
// Constructor
//------------------------------------------------------------------------------
BulletPhysicsMaterialManager::BulletPhysicsMaterialManager()
{
}

//------------------------------------------------------------------------------
// Method:    ~BulletPhysicsMaterialManager
// Parameter: void
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
BulletPhysicsMaterialManager::~BulletPhysicsMaterialManager()
{
	MaterialMap::iterator it = m_mMaterialMap.begin(), end = m_mMaterialMap.end();
	for (; it != end; ++it)
	{
		PhysicsMaterial* pMaterial = static_cast<PhysicsMaterial*>(it->second);
		delete pMaterial;
		pMaterial = NULL;
	}
	m_mMaterialMap.clear();
}

//------------------------------------------------------------------------------
// Method:    LoadMaterials
// Parameter: const std::string & p_strMaterialPath
// Returns:   void
// 
// Load physics materials.
//------------------------------------------------------------------------------
void BulletPhysicsMaterialManager::LoadMaterials(const std::string &p_strMaterialPath)
{
	TiXmlDocument doc(p_strMaterialPath.c_str());
	if (doc.LoadFile() == true)
	{
		// Look for the root "physics_materials" node
		TiXmlNode* pNode = doc.FirstChild("physics_materials");
		if (pNode != NULL)
		{
			// Iterate materials in the XML
			TiXmlNode* pMaterialNode = pNode->FirstChild();
			while (pMaterialNode != NULL)
			{
				const char* szNodeName = pMaterialNode->Value();
				float fRestitution = 0.5f;
				float fFriction = 0.25f;
				TiXmlElement* pElement = pMaterialNode->ToElement();
				pElement->QueryFloatAttribute("friction", &fFriction);
				pElement->QueryFloatAttribute("restitution", &fRestitution);

				PhysicsMaterial* pMaterial = new PhysicsMaterial(fRestitution, fFriction);

				// Add to our map
				m_mMaterialMap.insert(std::pair<std::string, PhysicsMaterial*>(std::string(szNodeName), pMaterial));
			
				// Next sibling
				pMaterialNode = pMaterialNode->NextSibling();
			}
		}
	}
}

//------------------------------------------------------------------------------
// Method:    GetMaterial
// Parameter: const std::string & p_strMaterialName
// Returns:   PhysicsMaterial*
// 
// Returns the material mapped to the given material name.
//------------------------------------------------------------------------------
BulletPhysicsMaterialManager::PhysicsMaterial* BulletPhysicsMaterialManager::GetMaterial(const std::string &p_strMaterialName)
{
	MaterialMap::iterator it = m_mMaterialMap.find(p_strMaterialName);
	if (it != m_mMaterialMap.end())
	{
		PhysicsMaterial* pMaterial = static_cast<PhysicsMaterial*>(it->second);
		return pMaterial;
	}

	return NULL;
}