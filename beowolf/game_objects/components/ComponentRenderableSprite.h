//------------------------------------------------------------------------
// ComponentRenderableSprite
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a renderable component that is a sprite.
//------------------------------------------------------------------------

#ifndef COMPONENTRENDERABLESPRITE_H
#define COMPONENTRENDERABLESPRITE_H

#include "W_Sprite.h"
#include "ComponentRenderable.h"

namespace week6
{
	class ComponentRenderableSprite : public Common::ComponentRenderable
	{
	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		ComponentRenderableSprite();
		virtual ~ComponentRenderableSprite();

		virtual const std::string ComponentID(){ return std::string("GOC_RenderableSprite"); }
		virtual void Update(float p_fDelta) {}
	
		//------------------------------------------------------------------------------
		// Public methods for "GOC_Renderable" family of components
		//------------------------------------------------------------------------------
		virtual void Init(const std::string &p_strTexturePath, const std::string &p_strVertexProgramPath, const std::string &p_strFragmentProgramPath, const glm::vec2& p_vDimensions);
		virtual void SyncTransform();
		wolf::Sprite* GetSprite() { return m_pSprite; }

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------

		wolf::Sprite* m_pSprite;
	};
}

#endif // COMPONENTRENDERABLESPRITE_H

