#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include <stdio.h>
#include <stdlib.h>
#ifndef __APPLE__
#include <GL/glew.h>
#endif
#include <GL/glfw.h>
#include "W_Common.h"
#include "Scene.h"
#include "BaseScene.h"
#include <glm/glm.hpp>
#include "math/W_Time.h"
#include "W_Input.h"

//========================================================================
// This is needed for newer versions of Visual Studio
//========================================================================
FILE _iob[] = { *stdin, *stdout, *stderr };

extern "C" FILE * __cdecl __iob_func(void)
{
	return _iob;
}
//========================================================================

void setupGraphics(const char* windowTitle, int windowWidth, int windowHeight)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}

	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	if (!glfwOpenWindow(windowWidth, windowHeight, 0, 0, 0, 0, 24, 0, GLFW_WINDOW))
	{
		fprintf(stderr, "Failed to open GLFW window\n");

		glfwTerminate();
		exit(EXIT_FAILURE);
	}

#ifndef __APPLE__
	glewExperimental = GL_TRUE;
	glewInit();
#endif

	glfwSetWindowTitle(windowTitle);

	// Ensure we can capture the escape key being pressed below
	glfwEnable(GLFW_STICKY_KEYS);

	// vsync
	glfwSwapInterval(1);
}

void updateGraphics()
{
	// set window bounds
	int width, height;
	glfwGetWindowSize(&width, &height);
	height = height > 0 ? height : 1;
	glViewport(0, 0, width, height);

	// clear screen
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void updateGameLogic(Scene* scene)
{
	// update game
	wolf::Time::Instance().update();
	wolf::Input::Instance().update();
	scene->Update();
	scene->Render();
}

int main()
{
    setupGraphics("Example", 1280, 720);
    
	BaseScene* scene = new BaseScene();
	scene->Init();

	while (glfwGetWindowParam(GLFW_OPENED))
	{
		updateGraphics();
		updateGameLogic(scene);

		// swap buffers
		glfwSwapBuffers();
	}
    
    glfwTerminate();
    exit( EXIT_SUCCESS );
}


