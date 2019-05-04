//-----------------------------------------------------------------------------
// File:			examples.h
// Original Author:	Gordon Wood
//
// Prototypes for all the example functions
//-----------------------------------------------------------------------------
#ifndef SCENE_H
#define SCENE_H

class Scene
{
public:
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
};

#endif
