#ifndef SCENE_H
#define SCENE_H

#include "W_Types.h"

class Scene
{
public:
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render(wolf::RenderTarget target) = 0;
	virtual void SetTex(wolf::RenderTarget target, unsigned int tex) = 0;
};

#endif
