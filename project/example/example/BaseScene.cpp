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
		std::cout << "is 4999 prime? " << wolf::Math::isPrime(4999) << std::endl;
	}

	void Update()
	{
	}

	void Render()
	{
	}
};


