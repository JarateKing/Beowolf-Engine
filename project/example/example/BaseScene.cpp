#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "Scene.h"
#include <iostream>

class BaseScene : public Scene
{
public:
	BaseScene()
	{
	}

	void Init()
	{
		std::cout << "example init" << std::endl;
	}

	void Update()
	{
		std::cout << "example update" << std::endl;
	}

	void Render()
	{
		std::cout << "example render" << std::endl;
	}
};


