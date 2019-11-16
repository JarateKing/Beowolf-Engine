#include "W_ResourceLoader.h"
#include <iostream>
#include <fstream>

namespace wolf
{
	std::string ResourceLoader::getTexture(std::string name) {
		std::string filename = "resources/textures/" + name;
		if (checkFileExists(filename))
			return filename;
		
		std::cout << "Failed to find texture " << name << "\n";
		return "resources/textures/fallback.tga";
	}

	std::string ResourceLoader::getVertexShader(std::string name) {
		std::string filename = "resources/shaders/" + name;
		if (checkFileExists(filename))
			return filename;

		std::cout << "Failed to find vertex shader " << name << "\n";
		return "resources/shaders/fallback.vsh";
	}

	std::string ResourceLoader::getPixelShader(std::string name) {
		std::string filename = "resources/shaders/" + name;
		if (checkFileExists(filename))
			return filename;

		std::cout << "Failed to find pixel shader " << name << "\n";
		return "resources/shaders/fallback.fsh";
	}

	std::string ResourceLoader::getModel(std::string name) {
		std::string filename = "resources/models/" + name;
		if (checkFileExists(filename))
			return filename;

		std::cout << "Failed to find model " << name << "\n";
		return "resources/models/teapot.bmw";
	}

	bool ResourceLoader::checkFileExists(std::string filename) {
		std::ifstream file(filename.c_str());
		return file.good();
	}
}