//------------------------------------------------------------------------
// ComponentRenderableSprite
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a renderable component that is a sprite.
//------------------------------------------------------------------------

#include "W_Model.h"
#include "ComponentRenderableSprite.h"
#include "GameObject.h"
#include "SceneManager.h"

using namespace week6;

//------------------------------------------------------------------------------
// Method:    ComponentRenderableSprite
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
ComponentRenderableSprite::ComponentRenderableSprite()
	:
	m_pSprite(NULL)
{
}

//------------------------------------------------------------------------------
// Method:    ~ComponentRenderableSprite
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
ComponentRenderableSprite::~ComponentRenderableSprite()
{
	// Remove the sprite from the scene
	Common::SceneManager::Instance()->RemoveSprite(m_pSprite);

	// Delete the sprite
	if (m_pSprite)
	{
		delete m_pSprite;
		m_pSprite = NULL;
	}
}

//------------------------------------------------------------------------------
// Method:    Init
// Parameter: const std::string & p_strTexturePath
// Parameter: const std::string & p_strVertexProgramPath
// Parameter: const std::string & p_strFragmentProgramPath
// Parameter: const glm::vec2& p_vDimensions
// Returns:   void
// 
// Initializes this component by loading the renderable object.
//------------------------------------------------------------------------------
void ComponentRenderableSprite::Init(const std::string &p_strTexturePath, const std::string &p_strVertexProgramPath, const std::string &p_strFragmentProgramPath, const glm::vec2& p_vDimensions)
{
	m_pSprite = new wolf::Sprite(p_strTexturePath, p_strVertexProgramPath, p_strFragmentProgramPath, p_vDimensions);
	Common::SceneManager::Instance()->AddSprite(m_pSprite);
}

//------------------------------------------------------------------------------
// Method:    SyncTransform
// Returns:   void
// 
// Syncs the sprite transform with the game object transform.
//------------------------------------------------------------------------------
void ComponentRenderableSprite::SyncTransform()
{
	m_pSprite->SetTransform(this->GetGameObject()->GetTransform().GetTransformation());
}
