//------------------------------------------------------------------------
// ComponentAnimController
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements an animation controller. It manages a list of 
// animations and applies them to a ComponentRenderable on the parent
// GameObject (if one exists).
//------------------------------------------------------------------------

#ifndef COMPNENTANIMCONTROLLER_H
#define COMPNENTANIMCONTROLLER_H

#include "ComponentBase.h"
#include <map>
#include "tinyxml.h"

namespace week2
{
	class ComponentRenderableMesh;

	class ComponentAnimController : public Common::ComponentBase
	{
	public:
		//------------------------------------------------------------------------------
		// Public types.
		//------------------------------------------------------------------------------

		// Struct to hold info about animation
		struct Anim
		{
			Anim(const std::string &p_strAnimName, const std::string &p_strResource, int p_iStartFrame, int p_iEndFrame, bool p_bLoop) : m_strAnimName(p_strAnimName), m_strResource(p_strResource), m_iStartFrame(p_iStartFrame), m_iEndFrame(p_iEndFrame), m_bLoop(p_bLoop) {}
			std::string m_strAnimName;
			std::string m_strResource;
			int m_iStartFrame;
			int m_iEndFrame;
			bool m_bLoop;
		};

		// Typedef for convenience
		typedef std::map<std::string, Anim*> AnimMap;

	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		ComponentAnimController(int p_iAnimSpeed = 30);
		virtual ~ComponentAnimController();

		virtual const std::string FamilyID() { return std::string("GOC_AnimController"); }
		virtual const std::string ComponentID(){ return std::string("GOC_AnimController"); }
		virtual void Update(float p_fDelta);

		static ComponentBase* ComponentAnimController::CreateComponent(TiXmlNode* p_node);

	public:
		//------------------------------------------------------------------------------
		// Public methods for "GOC_AnimController" family of components
		//------------------------------------------------------------------------------
		void AddAnim(const std::string &p_strAnimName, const std::string &p_strResourcePath, int p_iStartFrame, int p_iEndFrame, bool p_bLoop);
		void SetAnim(const std::string &p_strAnimName);

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------

		// Anim playback speed
		int m_iAnimSpeed;

		// Map of names to animations
		AnimMap m_animMap;
		
		// Current anim and frame
		const Anim* m_pCurrentAnim;
		float m_fAnimFrame;

		// Game object
		ComponentRenderableMesh* m_pRenderable;
	};
}

#endif // COMPNENTANIMCONTROLLER_H

