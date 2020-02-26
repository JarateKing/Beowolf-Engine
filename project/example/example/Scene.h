#ifndef SCENE_H
#define SCENE_H

class Scene
{
public:
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void SwitchShadowPass() = 0;
	virtual void SetTex(unsigned int depthMapTex) = 0;
};

#endif
