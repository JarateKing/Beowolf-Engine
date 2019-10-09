#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "BaseScene.h"
#include <iostream>

#include <fstream>
#include <string>

std::string readString(std::ifstream) {
	return "";
}

unsigned int readInt(std::ifstream) {
	return 0;
}

float readFloat(std::ifstream) {
	return 0.0;
}


BaseScene::BaseScene()
{
}

void BaseScene::Init()
{
}

void BaseScene::Update()
{
}

void BaseScene::Render()
{
}


