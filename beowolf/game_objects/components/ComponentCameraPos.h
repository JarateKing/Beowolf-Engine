//------------------------------------------------------------------------
// ComponentCharacterController
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a character controller. It polls input and sends
// movement instructions to the relevant sibling components.
//------------------------------------------------------------------------

#ifndef COMPONENTCAMERAPOS_H
#define COMPONENTCAMERAPOS_H

#include "ComponentBase.h"
#include "SceneCamera.h"

namespace week2
{
	class ComponentCameraPos : public Common::ComponentBase
	{
	public:
		//------------------------------------------------------------------------------
		// Public types.
		//------------------------------------------------------------------------------

	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		ComponentCameraPos();
		virtual ~ComponentCameraPos();

		virtual const std::string FamilyID() { return std::string("GOC_CameraPos"); }
		virtual const std::string ComponentID(){ return std::string("GOC_CameraPos"); }
		virtual void Update(float p_fDelta);
		virtual void SetCamera(Common::SceneCamera* p_camera);
		virtual void SetEnabled(bool setting);

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------
		Common::SceneCamera* m_camera;
		bool isEnabled;
		bool isEnabledDirty;
	};
}

#endif // COMPONENTCAMERAPOS_H

