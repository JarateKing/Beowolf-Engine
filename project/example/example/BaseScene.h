//-----------------------------------------------------------------------------
// File:			examples.h
// Original Author:	Gordon Wood
//
// Prototypes for all the example functions
//-----------------------------------------------------------------------------
#ifndef BASESCENE_H
#define BASESCENE_H

#include "Scene.h"

class BaseScene : public Scene
{
public:
	BaseScene();
	void Init();
	void Update();
	void Render();
};

#endif