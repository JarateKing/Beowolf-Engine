#include "BMWLoader.h"
#include <iostream>

namespace wolf
{
	void BMWLoader::loadFile(std::string file, std::vector<std::string>* texlist, std::vector<std::vector<Vertex>>* meshlist, std::vector<std::vector<unsigned int>>* indexlist, BMWNode* root, std::map<int, BMWNode*>* nodeIDs, std::map<int, std::vector<std::pair<int, float>>>* boneWeights) {
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
			unsigned int bonedVertices = readInt(&in);
			for (int j = 0; j < bonedVertices; j++) {
				int boneVertexID = readInt(&in);
				int boneWeightNum = readInt(&in);
				for (int k = 0; k < boneWeightNum; k++) {
					(*boneWeights)[boneVertexID].push_back({readInt(&in), readFloat(&in)});
				}
			}

			unsigned int vertices = readInt(&in);
			(*meshlist).push_back(std::vector<Vertex>());
			for (int j = 0; j < vertices; j++) {
				Vertex cur = { readFloat(&in), readFloat(&in), readFloat(&in), readInt(&in), readInt(&in), readInt(&in), readInt(&in), readFloat(&in), readFloat(&in) };
				cur.normalDirX = readFloat(&in);
				cur.normalDirY = readFloat(&in);
				cur.normalDirZ = readFloat(&in);
				(*meshlist)[i].push_back(cur);
			}
			unsigned int indices = readInt(&in);
			(*indexlist).push_back(std::vector<unsigned int>());
			for (int j = 0; j < indices; j++) {
				for (int k = 0; k < 3; k++) {
					(*indexlist)[i].push_back(readInt(&in));
				}
			}
		}

		*root = readNode(&in, nodeIDs);
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

	glm::mat4 BMWLoader::readTransform(std::ifstream* in) {
		glm::mat4 transform;

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				transform[i][j] = readFloat(in);

		return transform;
	}

	BMWNode BMWLoader::readNode(std::ifstream* in, std::map<int, BMWNode*>* nodeIDs) {
		unsigned int nodeId = readInt(in);
		
		glm::mat4 transform = readTransform(in);

		unsigned int meshNum = readInt(in);
		unsigned int* meshes = new unsigned int[meshNum];
		for (int i = 0; i < meshNum; i++)
			meshes[i] = readInt(in);

		unsigned int childNum = readInt(in);
		std::vector<BMWNode> children;

		for (int i = 0; i < childNum; i++) {
			children.push_back(readNode(in, nodeIDs));
		}

		BMWNode toret;
		toret.transform = transform;
		toret.meshNum = meshNum;
		toret.meshIDs = meshes;
		toret.children = children;

		(*nodeIDs)[nodeId] = &toret;
		return toret;
	}
}