#include "W_ResourceLoader.h"
#include <fstream>

namespace wolf
{
	std::string ResourceLoader::getTexture(std::string name) {
		return "resources/textures/" + name;
	}

	std::string ResourceLoader::getVertexShader(std::string name) {
		return "resources/shaders/" + name;
	}

	std::string ResourceLoader::getPixelShader(std::string name) {
		return "resources/shaders/" + name;
	}

	std::string ResourceLoader::getModel(std::string name) {
		return "resources/models/" + name;
	}

	bool ResourceLoader::checkFileExists(std::string filename) {
		std::ifstream file(filename.c_str());
		return file.good();
	}
}