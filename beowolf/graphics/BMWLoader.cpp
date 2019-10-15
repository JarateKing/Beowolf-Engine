#include "BMWLoader.h"

namespace wolf
{
	void BMWLoader::loadFile(std::string file, std::vector<std::string>* texlist, std::vector<std::vector<Vertex>>* meshlist, std::vector< std::vector<unsigned int*>>* indexlist, BMWNode* root) {
		std::ifstream in(file, std::ifstream::binary);

		readString(&in);
		unsigned int materials = readInt(&in);
		for (int i = 0; i < materials; i++) {
			unsigned int textures = readInt(&in);
			for (int j = 0; j < textures; j++) {
				(*texlist).push_back(readString(&in));
			}
		}

		unsigned int meshes = readInt(&in);
		for (int i = 0; i < meshes; i++) {
			unsigned int vertices = readInt(&in);
			(*meshlist).push_back(std::vector<Vertex>(vertices));
			for (int j = 0; j < vertices; j++) {
				Vertex cur = { readFloat(&in), readFloat(&in), readFloat(&in), readInt(&in), readInt(&in), readInt(&in), readInt(&in), readFloat(&in), readFloat(&in) };
				cur.normalDirX = readFloat(&in);
				cur.normalDirY = readFloat(&in);
				cur.normalDirZ = readFloat(&in);
				(*meshlist)[i].push_back(cur);
			}
			unsigned int indices = readInt(&in);
			(*indexlist).push_back(std::vector<unsigned int*>(indices));
			for (int j = 0; j < indices; j++) {
				unsigned int* cur = new unsigned int[3];
				for (int k = 0; k < 3; k++)
					cur[k] = readInt(&in);
				(*indexlist)[i].push_back(cur);
			}
		}

		*root = readNode(&in);
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