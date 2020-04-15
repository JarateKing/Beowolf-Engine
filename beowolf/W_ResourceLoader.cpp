#include "W_ResourceLoader.h"
#include <iostream>
#include <fstream>
#include <algorithm>

#define FALLBACK_TEXTURE_TGA "resources/textures/fallback.tga"
#define FALLBACK_TEXTURE_DDS "resources/textures/fallback.dds"
#define FALLBACK_VERTEXSHADER "resources/shaders/fallback.vsh"
#define FALLBACK_PIXELSHADER "resources/shaders/fallback.fsh"
#define FALLBACK_MODEL "resources/models/teapot.bmw"
#define FALLBACK_FONT "resources/fonts/opensans_semibold.fnt"
#define FALLBACK_JSON "resources/objects/default.json"

namespace wolf
{
	std::string ResourceLoader::getTexture(const std::string& name) {
		std::string filename = "resources/textures/" + name;
		if (checkFileExists(filename))
			return filename;
		
		std::cout << "Failed to find texture " << name << "\n";
		return (name.substr(std::max(0, (int)(name.length()) - 4)) != ".dds") ? FALLBACK_TEXTURE_TGA : FALLBACK_TEXTURE_DDS;
	}

	std::string ResourceLoader::getModelTexture(const std::string& name) {
		std::string filename = "resources/models/" + name;
		if (checkFileExists(filename))
			return filename;

		std::cout << "Failed to find texture " << name << "\n";
		return (name.substr(std::max(0, (int)(name.length()) - 4)) != ".dds") ? FALLBACK_TEXTURE_TGA : FALLBACK_TEXTURE_DDS;
	}

	std::pair<std::string, std::string> ResourceLoader::getShaders(const std::string& name) {
		return { getVertexShader(name + ".vsh"), getPixelShader(name + ".fsh") };
	}

	std::string ResourceLoader::getVertexShader(const std::string& name) {
		std::string filename = "resources/shaders/" + name;
		if (checkFileExists(filename))
			return filename;

		std::cout << "Failed to find vertex shader " << name << "\n";
		return FALLBACK_VERTEXSHADER;
	}

	std::string ResourceLoader::getPixelShader(const std::string& name) {
		std::string filename = "resources/shaders/" + name;
		if (checkFileExists(filename))
			return filename;

		std::cout << "Failed to find pixel shader " << name << "\n";
		return FALLBACK_PIXELSHADER;
	}

	std::string ResourceLoader::getModel(const std::string& name) {
		std::string filename = "resources/models/" + name;
		if (checkFileExists(filename))
			return filename;

		std::cout << "Failed to find model " << name << "\n";
		return FALLBACK_MODEL;
	}

	std::string ResourceLoader::getFont(const std::string& name) {
		std::string filename = "resources/fonts/" + name;
		if (checkFileExists(filename))
			return filename;

		std::cout << "Failed to find font " << name << "\n";
		return FALLBACK_FONT;
	}

	std::string ResourceLoader::getJSONObject(const std::string& name) {
		std::string filename = "resources/objects/" + name;
		if (checkFileExists(filename))
			return filename;

		std::cout << "Failed to find object " << name << "\n";
		return FALLBACK_JSON;
	}

	bool ResourceLoader::checkFileExists(const std::string& filename) {
		std::ifstream file(filename.c_str());
		return file.good();
	}
}