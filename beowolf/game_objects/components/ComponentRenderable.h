//------------------------------------------------------------------------
// ComponentRenderable
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class provides the base class for all renderable-type components.
//------------------------------------------------------------------------

#ifndef COMPONENTRENDERABLE_H
#define COMPONENTRENDERABLE_H

#include "W_Model.h"
#include "ComponentBase.h"

namespace Common
{
	class ComponentRenderable : public Common::ComponentBase
	{
	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		ComponentRenderable() {}
		virtual ~ComponentRenderable() {}

		virtual const std::string FamilyID() { return std::string("GOC_Renderable"); }
		virtual void SyncTransform() = 0;

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------
	};
}

#endif // COMPONENTRENDERABLE_H