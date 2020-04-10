#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include <stdio.h>
#include <stdlib.h>
#ifndef __APPLE__
#include <GL/glew.h>
#endif
#include <GL/glfw.h>

#include "W_Common.h"
#include "BaseScene.h"
#include "W_Time.h"

//========================================================================
// This is needed for newer versions of Visual Studio
//========================================================================
FILE _iob[] = { *stdin, *stdout, *stderr };

unsigned int depthMapFrameBuf;
unsigned int depthMapTex;
unsigned int depthMapFrameBuf2;
unsigned int depthMapTex2;
const unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 2048;

unsigned int depthFieldMapBuf;
unsigned int depthFieldMapTex;

unsigned int reflectionFrameBuf;
unsigned int reflectionRenderBuf;
unsigned int reflectionTex;
const unsigned int REFLECTION_WIDTH = 512, REFLECTION_HEIGHT = 512;

unsigned int refractionFrameBuf;
unsigned int refractionRenderBuf;
unsigned int refractionTex;
const unsigned int REFRACTION_WIDTH = 512, REFRACTION_HEIGHT = 512;

unsigned int fogFrameBuf;
unsigned int fogRenderBuf;
unsigned int fogTex;
const unsigned int FOG_WIDTH = 512, FOG_HEIGHT = 512;

unsigned int postFrameBuf1;
unsigned int postFrameBuf2;
unsigned int postDepthBuf1;
unsigned int postDepthBuf2;
unsigned int postTex1;
unsigned int postTex2;
const unsigned int POST_TEX_WIDTH = 2048, POST_TEX_HEIGHT = 1024;

extern "C" FILE * __cdecl __iob_func(void)
{
	return _iob;
}
//========================================================================

void InitializeRenderTarget(unsigned int* frameBuf, unsigned int* tex, unsigned int width, unsigned int height, int blendType, int format) {
	glGenFramebuffers(1, frameBuf);
	glGenTextures(1, tex);

	glBindTexture(GL_TEXTURE_2D, *tex);
	if (format == 0)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	else if (format == 1)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_SHORT, NULL);
	else if (format == 2)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, blendType);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, blendType);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, *frameBuf);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *tex, 0);
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void InitializeRenderTarget(unsigned int* frameBuf, unsigned int* tex, unsigned int* renderBuf, unsigned int width, unsigned int height, int blendType, int format) {
	glGenFramebuffers(1, frameBuf);
	glGenTextures(1, tex);
	glGenRenderbuffers(1, renderBuf);

	glBindTexture(GL_TEXTURE_2D, *tex);
	
	if (format == 0)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	else if (format == 1)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_SHORT, NULL);
	else if (format == 2)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, blendType);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, blendType);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindRenderbuffer(GL_RENDERBUFFER, *renderBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, *frameBuf);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *tex, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *renderBuf);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RenderTarget(Scene* scene, unsigned int frameBuf, unsigned int width, unsigned int height, unsigned int clearBits, wolf::RenderTarget target) {
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuf);
	glViewport(0, 0, width, height);
	if (clearBits != 0)
		glClear(clearBits);
	scene->Render(target);
}

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

	InitializeRenderTarget(&depthMapFrameBuf, &depthMapTex, SHADOW_WIDTH, SHADOW_HEIGHT, GL_LINEAR, 0);
	InitializeRenderTarget(&depthMapFrameBuf2, &depthMapTex2, SHADOW_WIDTH, SHADOW_HEIGHT, GL_NEAREST, 0);
	InitializeRenderTarget(&reflectionFrameBuf, &reflectionTex, &reflectionRenderBuf, REFLECTION_WIDTH, REFLECTION_HEIGHT, GL_LINEAR, 1);
	InitializeRenderTarget(&refractionFrameBuf, &refractionTex, &refractionRenderBuf, REFRACTION_WIDTH, REFRACTION_HEIGHT, GL_LINEAR, 1);
	InitializeRenderTarget(&fogFrameBuf, &fogTex, FOG_WIDTH, FOG_HEIGHT, GL_NEAREST, 0);
	InitializeRenderTarget(&postFrameBuf1, &postTex1, &postDepthBuf1, POST_TEX_WIDTH, POST_TEX_HEIGHT, GL_LINEAR, 2);
	InitializeRenderTarget(&postFrameBuf2, &postTex2, &postDepthBuf2, POST_TEX_WIDTH, POST_TEX_HEIGHT, GL_LINEAR, 2);
}

void updateGraphics(Scene* scene)
{
	// set window bounds
	int width, height;
	glfwGetWindowSize(&width, &height);
	height = height > 0 ? height : 1;
	glViewport(0, 0, width, height);

	glBindTexture(GL_TEXTURE_2D, postTex1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, postTex2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, depthFieldMapTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, FOG_WIDTH, FOG_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, postDepthBuf1);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, postDepthBuf2);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// clear screen
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// perform renders
	glCullFace(GL_FRONT);

	RenderTarget(scene, depthMapFrameBuf, SHADOW_WIDTH, SHADOW_HEIGHT, GL_DEPTH_BUFFER_BIT, wolf::RenderTarget::ShadowDepthmap);
	RenderTarget(scene, fogFrameBuf, FOG_WIDTH, FOG_HEIGHT, GL_DEPTH_BUFFER_BIT, wolf::RenderTarget::WaterFog);

	glCullFace(GL_BACK);

	RenderTarget(scene, reflectionFrameBuf, REFLECTION_WIDTH, REFLECTION_HEIGHT, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, wolf::RenderTarget::WaterReflection);
	RenderTarget(scene, refractionFrameBuf, REFRACTION_WIDTH, REFRACTION_HEIGHT, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, wolf::RenderTarget::WaterRefraction);
	RenderTarget(scene, postFrameBuf1, width, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, wolf::RenderTarget::PostProcessing);
	RenderTarget(scene, depthMapFrameBuf2, SHADOW_WIDTH, SHADOW_HEIGHT, GL_DEPTH_BUFFER_BIT, wolf::RenderTarget::Characters);
	RenderTarget(scene, postFrameBuf1, width, height, 0, wolf::RenderTarget::GrayScale);
	RenderTarget(scene, postFrameBuf2, width, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, wolf::RenderTarget::Blur);
	RenderTarget(scene, depthMapFrameBuf, SHADOW_WIDTH, SHADOW_HEIGHT, GL_DEPTH_BUFFER_BIT, wolf::RenderTarget::DepthFieldMap);
	RenderTarget(scene, 0, width, height, 0, wolf::RenderTarget::DepthOfField);
	RenderTarget(scene, 0, width, height, 0, wolf::RenderTarget::HUD);
}

void updateGameLogic(Scene* scene)
{
	// update game
	wolf::Time::Instance().update();
	wolf::Input::Instance().update();
	scene->Update();
}

int main()
{
    setupGraphics("Example", 1280, 720);
    
	BaseScene* scene = new BaseScene();
	scene->Init();
	scene->SetTex(wolf::RenderTarget::ShadowDepthmap, depthMapTex);
	scene->SetTex(wolf::RenderTarget::Cutouts, depthMapTex2);
	scene->SetTex(wolf::RenderTarget::WaterReflection, reflectionTex);
	scene->SetTex(wolf::RenderTarget::PostProcessing, postTex1);
	scene->SetTex(wolf::RenderTarget::Blur, postTex2);
	scene->SetTex(wolf::RenderTarget::DepthFieldMap, depthFieldMapTex);
	scene->SetTex(wolf::RenderTarget::WaterRefraction, refractionTex);
	scene->SetTex(wolf::RenderTarget::WaterFog, fogTex);

	while (glfwGetWindowParam(GLFW_OPENED))
	{
		updateGameLogic(scene);
		updateGraphics(scene);

		// swap buffers
		glfwSwapBuffers();
	}
    
    glfwTerminate();
    exit( EXIT_SUCCESS );
}


