#include "BMWLoader.h"
#include <iostream>

namespace wolf
{
	void BMWLoader::loadFile(std::string file) {
		std::ifstream in(file, std::ifstream::binary);

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

		BMWNode root = readNode(&in);
	}

	std::string BMWLoader::readString(std::ifstream* in) {
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

	unsigned int BMWLoader::readInt(std::ifstream* in) {
		unsigned int toret;
		(*in).read((char*)&toret, sizeof(unsigned int));
		return toret;
	}

	float BMWLoader::readFloat(std::ifstream* in) {
		float toret;
		(*in).read((char*)&toret, sizeof(float));
		return toret;
	}

	BMWNode BMWLoader::readNode(std::ifstream* in) {
		glm::mat4 transform;

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				transform[i][j] = readFloat(in);

		unsigned int meshNum = readInt(in);
		unsigned int* meshes = new unsigned int[meshNum];
		for (int i = 0; i < meshNum; i++)
			meshes[i] = readInt(in);

		unsigned int childNum = readInt(in);
		std::vector<BMWNode> children;

		for (int i = 0; i < childNum; i++) {
			children.push_back(readNode(in));
		}

		BMWNode toret;
		toret.transform = transform;
		toret.meshNum = meshNum;
		toret.meshIDs = meshes;
		toret.children = children;
		return toret;
	}
}