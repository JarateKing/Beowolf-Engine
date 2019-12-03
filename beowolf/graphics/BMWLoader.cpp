#include "BMWLoader.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include "JSON/json.hpp"

namespace wolf
{
	bool boneWeightCompare(std::pair<int, float> a, std::pair<int, float> b) {
		if (a.second == b.second) return a.first > b.first;
		return a.second > b.second;
	}

	BMWNode* BMWLoader::loadFile(std::string file, std::vector<std::string>& texlist, std::vector<std::vector<Vertex>>& meshlist, std::vector<std::vector<unsigned int>>& indexlist, std::map<int, BMWNode*>& nodeIDs, std::map<int, std::vector<std::pair<int, float>>>& boneWeights, std::vector<BMWAnim*>& animlist, std::map<std::string, BMWAnimSegment*>& animations, std::string& defaultAnim) {
		if (m_stored.count(file)) {
			BMWModeLData memo = m_stored[file];
			texlist = memo.texlist;
			meshlist = memo.meshlist;
			indexlist = memo.indexlist;
			nodeIDs = memo.nodeIDs;
			boneWeights = memo.boneWeights;
			animlist = memo.animlist;
			animations = memo.animations;
			defaultAnim = memo.defaultAnim;
			std::cout << "using " << &memo.meshlist << " " << memo.meshlist.size() << "\n";
			return memo.root;
		}
		
		std::ifstream in(file, std::ifstream::binary);
		
		std::string jsonFile = file;
		if (jsonFile.substr(jsonFile.size() - 4) == ".bmw")
			jsonFile = jsonFile.substr(0, jsonFile.size() - 4) + ".json";

		std::ifstream jsonIn(jsonFile);
		if (jsonIn) {
			std::stringstream jsonFileStream;
			jsonFileStream << jsonIn.rdbuf();
			std::string jsonFileData = jsonFileStream.str();
			nlohmann::json jsonData = nlohmann::json::parse(jsonFileData);
			
			std::string defaultAnimStr = jsonData["defaultAnim"];
			defaultAnim = defaultAnimStr;

			for (auto anim : jsonData["clips"]) {
				BMWAnimSegment* clip = new BMWAnimSegment;
				clip->anim = 0;
				clip->start = anim["start"];
				clip->end = anim["end"];
				clip->isLoop = anim["loop"];
				std::string clipName = anim["name"];

				(animations)[clipName] = clip;
			}
		}

		readString(&in);
		unsigned int materials = readInt(&in);
		for (int i = 0; i < materials; i++) {
			unsigned int textures = readInt(&in);
			for (int j = 0; j < textures; j++) {
				(texlist).push_back(readString(&in));
			}
		}

		unsigned int anims = readInt(&in);
		for (int i = 0; i < anims; i++) {
			unsigned int duration = readInt(&in);
			unsigned int speed = readInt(&in);
			unsigned int bones = readInt(&in);

			std::map<int, std::vector<glm::mat4>> trans;
			for (int j = 0; j < bones; j++) {
				unsigned int boneAffected = readInt(&in);
				std::vector<glm::mat4> transforms;
				for (int k = 0; k <= duration; k++) {
					transforms.push_back(readTransform(&in));
				}
				trans[boneAffected] = transforms;
			}

			BMWAnim* current = new BMWAnim();
			current->duration = duration;
			current->rate = speed;
			current->transforms = trans;
			animlist.push_back(current);
		}

		unsigned int meshes = readInt(&in);
		for (int i = 0; i < meshes; i++) {
			unsigned int bonedVertices = readInt(&in);
			for (int j = 0; j < bonedVertices; j++) {
				int boneVertexID = readInt(&in);
				int boneWeightNum = readInt(&in);
				for (int k = 0; k < boneWeightNum; k++) {
					(boneWeights)[boneVertexID].push_back({readInt(&in), readFloat(&in)});
				}
				std::sort((boneWeights)[boneVertexID].begin(), (boneWeights)[boneVertexID].end(), boneWeightCompare);
			}

			unsigned int vertices = readInt(&in);
			(meshlist).push_back(std::vector<Vertex>());
			for (int j = 0; j < vertices; j++) {
				Vertex cur = { readFloat(&in), readFloat(&in), readFloat(&in), readInt(&in), readInt(&in), readInt(&in), readInt(&in), readFloat(&in), readFloat(&in) };
				cur.normalDirX = readFloat(&in);
				cur.normalDirY = readFloat(&in);
				cur.normalDirZ = readFloat(&in);
				glm::vec4 boneIndices = glm::vec4();
				glm::vec4 boneWeight = glm::vec4();
				for (int k = 0; k < (boneWeights)[j].size() && k < 4; k++) {
					boneIndices[k] = (boneWeights)[j][k].first;
					boneWeight[k] = (boneWeights)[j][k].second;
				}
				cur.boneIndices = boneIndices;
				cur.boneWeights = boneWeight;
				(meshlist)[i].push_back(cur);
			}
			unsigned int indices = readInt(&in);
			(indexlist).push_back(std::vector<unsigned int>());
			for (int j = 0; j < indices; j++) {
				for (int k = 0; k < 3; k++) {
					(indexlist)[i].push_back(readInt(&in));
				}
			}
		}

		BMWNode* root = readNode(&in, &nodeIDs);

		m_stored[file].texlist = std::vector<std::string>(texlist);
		m_stored[file].meshlist = std::vector<std::vector<Vertex>>(meshlist);
		m_stored[file].indexlist = std::vector<std::vector<unsigned int>>(indexlist);
		m_stored[file].root = root;
		m_stored[file].nodeIDs = std::map<int, BMWNode*>(nodeIDs);
		m_stored[file].boneWeights = std::map<int, std::vector<std::pair<int, float>>>(boneWeights);
		m_stored[file].animlist = std::vector<BMWAnim*>(animlist);
		m_stored[file].animations = std::map<std::string, BMWAnimSegment*>(animations);
		m_stored[file].defaultAnim = defaultAnim;

		std::cout << "storing " << &meshlist << "\n";
		std::cout << "stored " << &m_stored[file].meshlist << "\n";

		return root;
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

	BMWNode* BMWLoader::readNode(std::ifstream* in, std::map<int, BMWNode*>* nodeIDs) {
		unsigned int nodeId = readInt(in);
		
		glm::mat4 transform = readTransform(in);

		unsigned int meshNum = readInt(in);
		unsigned int* meshes = new unsigned int[meshNum];
		for (int i = 0; i < meshNum; i++)
			meshes[i] = readInt(in);

		unsigned int childNum = readInt(in);
		std::vector<BMWNode*> children(childNum);

		for (int i = 0; i < childNum; i++) {
			children[i] = readNode(in, nodeIDs);
		}

		std::cout << "a\n";
		BMWNode* toret = new BMWNode();
		toret->transform = transform;
		toret->meshNum = meshNum;
		toret->meshIDs = meshes;
		toret->children = children;

		(*nodeIDs).insert({ nodeId, toret });
		return toret;
	}
}