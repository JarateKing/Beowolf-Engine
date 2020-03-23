#ifndef SCENE_H
#define SCENE_H

enum RenderTarget {
	Screen,
	HUD,
	ShadowDepthmap,
	DepthFieldMap,
	WaterReflection,
	PostProcessing,
	GrayScale,
	DepthOfField,
	Blur,
	WaterRefraction,
	WaterFog,
	Characters,
	Cutouts,
};

class Scene
{
public:
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render(RenderTarget target) = 0;
	virtual void SetTex(RenderTarget target, unsigned int tex) = 0;
};

#endif
