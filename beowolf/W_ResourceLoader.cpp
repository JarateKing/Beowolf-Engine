#include "W_ResourceLoader.h"
#include <fstream>

namespace wolf
{
	std::string ResourceLoader::getTexture(std::string name) {
		std::string filename = "resources/textures/" + name;
		if (checkFileExists(filename))
			return filename;
		else
			return "resources/textures/fallback.tga";
	}

	std::string ResourceLoader::getVertexShader(std::string name) {
		std::string filename = "resources/shaders/" + name;
		if (checkFileExists(filename))
			return filename;
		else
			return "resources/shaders/fallback.vsh";
	}

	std::string ResourceLoader::getPixelShader(std::string name) {
		std::string filename = "resources/shaders/" + name;
		if (checkFileExists(filename))
			return filename;
		else
			return "resources/shaders/fallback.fsh";
	}

	std::string ResourceLoader::getModel(std::string name) {
		return "resources/models/" + name;
	}

	bool ResourceLoader::checkFileExists(std::string filename) {
		std::ifstream file(filename.c_str());
		return file.good();
	}
}