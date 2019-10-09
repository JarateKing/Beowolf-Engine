#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "BaseScene.h"
#include <iostream>

#include <fstream>
#include <string>

std::string readString(std::ifstream* in) {
	std::string toret = "";
	char next;

	do {
		(*in).read(&next, sizeof(char));
		toret += next;
	} while (next != 0);

	return toret;
}

unsigned int readInt(std::ifstream* in) {
	return 0;
}

float readFloat(std::ifstream* in) {
	return 0.0;
}


BaseScene::BaseScene()
{
}

void BaseScene::Init()
{
	std::ifstream in("resources/models/teapot.bmw", std::ifstream::binary);
	std::cout << readString(&in);
}

void BaseScene::Update()
{
}

void BaseScene::Render()
{
}


