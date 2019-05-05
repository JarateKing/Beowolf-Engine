#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "Scene.h"
#include <iostream>
#include "W_Input.h"

class BaseScene : public Scene
{
public:
	BaseScene()
	{
	}

	void Init()
	{
	}

	void Update()
	{
		if (wolf::Input::Instance().isKeyPressed(INPUT_KB_A))
		{
			std::cout << "pressed key" << std::endl;
		}
		if (wolf::Input::Instance().isKeyHeld(INPUT_KB_A, 0.5))
		{
			std::cout << "held key for " << wolf::Input::Instance().getKey(INPUT_KB_A) << " seconds" << std::endl;
		}

		if (wolf::Input::Instance().isAfk(0.5))
		{
			std::cout << "time afk: " << wolf::Input::Instance().getTimeAfk() << "s" << std::endl;
		}
	}

	void Render()
	{
	}
};


