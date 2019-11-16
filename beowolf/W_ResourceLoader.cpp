#include "W_ResourceLoader.h"
#include <iostream>
#include <fstream>

#define FALLBACK_TEXTURE "resources/textures/fallback.tga"
#define FALLBACK_VERTEXSHADER "resources/shaders/fallback.vsh"
#define FALLBACK_PIXELSHADER "resources/shaders/fallback.fsh"
#define FALLBACK_MODEL "resources/models/teapot.bmw"

namespace wolf
{
	std::string ResourceLoader::getTexture(std::string name) {
		std::string filename = "resources/textures/" + name;
		if (checkFileExists(filename))
			return filename;
		
		std::cout << "Failed to find texture " << name << "\n";
		return FALLBACK_TEXTURE;
	}

	std::string ResourceLoader::getVertexShader(std::string name) {
		std::string filename = "resources/shaders/" + name;
		if (checkFileExists(filename))
			return filename;

		std::cout << "Failed to find vertex shader " << name << "\n";
		return FALLBACK_VERTEXSHADER;
	}

	std::string ResourceLoader::getPixelShader(std::string name) {
		std::string filename = "resources/shaders/" + name;
		if (checkFileExists(filename))
			return filename;

		std::cout << "Failed to find pixel shader " << name << "\n";
		return FALLBACK_PIXELSHADER;
	}

	std::string ResourceLoader::getModel(std::string name) {
		std::string filename = "resources/models/" + name;
		if (checkFileExists(filename))
			return filename;

		std::cout << "Failed to find model " << name << "\n";
		return FALLBACK_MODEL;
	}

	bool ResourceLoader::checkFileExists(std::string filename) {
		std::ifstream file(filename.c_str());
		return file.good();
	}
}