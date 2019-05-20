#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "Scene.h"
#include <iostream>
#include "W_Math.h"

class BaseScene : public Scene
{
public:
	BaseScene()
	{
	}

	void Init()
	{
		std::cout << "lerp: " << wolf::Math::lerp(5.0f, 7.0f, 0.5f);
	}

	void Update()
	{
	}

	void Render()
	{
	}
};


