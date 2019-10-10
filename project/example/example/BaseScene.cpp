#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "BaseScene.h"
#include <iostream>

#include <fstream>
#include <string>
#include <W_Common.h>
#include <vector>

struct Node {
	glm::mat4 transform;
	unsigned int meshNum;
	unsigned int* meshIDs;
	std::vector<Node> children;
};

std::string readString(std::ifstream* in) {
	std::string toret = "";
	char next;

	while (true) {
		(*in).read(&next, sizeof(char));
		if (next != 0)
			toret += next;
		else
			break;
	}

	return toret;
}

unsigned int readInt(std::ifstream* in) {
	unsigned int toret;
	(*in).read((char*)&toret, sizeof(unsigned int));
	return toret;
}

float readFloat(std::ifstream* in) {
	float toret;
	(*in).read((char*)&toret, sizeof(float));
	return toret;
}

Node readNode(std::ifstream* in) {
	glm::mat4 transform;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			transform[i][j] = readFloat(in);

	unsigned int meshNum = readInt(in);
	unsigned int* meshes = new unsigned int[meshNum];
	for (int i = 0; i < meshNum; i++)
		meshes[i] = readInt(in);

	unsigned int childNum = readInt(in);
	std::vector<Node> children;

	for (int i = 0; i < childNum; i++) {
		children.push_back(readNode(in));
	}

	Node toret;
	toret.transform = transform;
	toret.meshNum = meshNum;
	toret.meshIDs = meshes;
	toret.children = children;
	return toret;
}


BaseScene::BaseScene()
{
}

void BaseScene::Init()
{
	std::ifstream in("resources/models/teapot.bmw", std::ifstream::binary);

	readString(&in);
	unsigned int materials = readInt(&in);
	for (int i = 0; i < materials; i++) {
		unsigned int textures = readInt(&in);
		for (int j = 0; j < textures; j++) {
			std::cout << readString(&in) << "\n";
		}
	}

	unsigned int meshes = readInt(&in);
	for (int i = 0; i < meshes; i++) {
		unsigned int vertices = readInt(&in);
		for (int j = 0; j < vertices; j++) {
			std::cout << readFloat(&in) << " " << readFloat(&in) << " " << readFloat(&in) << " ";
			std::cout << readInt(&in) << " " << readInt(&in) << " " << readInt(&in) << " " << readInt(&in) << " ";
			std::cout << readFloat(&in) << " " << readFloat(&in) << " ";
			std::cout << readFloat(&in) << " " << readFloat(&in) << " " << readFloat(&in) << "\n";
		}
	}

	Node root = readNode(&in);
}

void BaseScene::Update()
{
}

void BaseScene::Render()
{
}


