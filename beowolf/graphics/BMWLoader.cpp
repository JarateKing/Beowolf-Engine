#include "BMWLoader.h"
#include "W_ResourceLoader.h"
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

	BMWModeLData* BMWLoader::loadFile(const std::string& file) {
		// if this file was already loaded, use the cached version
		if (m_stored.count(file)) {
			return &m_stored[file];
		}

		// create containers for data
		std::vector<std::string> texlist;
		std::vector<std::vector<Vertex>> meshlist;
		std::vector<std::vector<unsigned int>> indexlist;
		std::map<int, BMWNode*> nodeIDs;
		std::map<int, std::vector<std::pair<int, float>>> boneWeights;
		std::vector<BMWAnim*> animlist;
		std::map<std::string, BMWAnimSegment*> animations;
		std::string defaultAnim;
		glm::mat4 transformModel;
		std::vector<std::pair<int, std::string>> boneNames;
		
		// attempt to load json file by same name
		std::ifstream in(file, std::ifstream::binary);
		std::string jsonFile = file;
		if (jsonFile.substr(jsonFile.size() - 4) == ".bmw")
			jsonFile = jsonFile.substr(0, jsonFile.size() - 4) + ".json";

		// load override settings from json
		int animCountOffset = 0;
		std::vector<std::string> subfilesLoaded;
		std::vector<std::string> texOverrides;
		std::ifstream jsonIn(jsonFile);
		nlohmann::json jsonData;
		if (jsonIn) {
			std::stringstream jsonFileStream;
			jsonFileStream << jsonIn.rdbuf();
			std::string jsonFileData = jsonFileStream.str();
			jsonData = nlohmann::json::parse(jsonFileData);
			
			defaultAnim = "idle";
			if (jsonData.contains("defaultAnim")) {
				std::string defaultAnimStr = jsonData["defaultAnim"];
				defaultAnim = defaultAnimStr;
			}

			float rotation = 0.0f;
			glm::vec3 rotationAngle = glm::vec3(0, 1, 0);
			glm::vec3 translation;
			glm::vec3 scale = glm::vec3(1, 1, 1);
			if (jsonData.contains("rotation")) {
				rotation = jsonData["rotation"];
			}
			if (jsonData.contains("rotationAngle")) {
				std::string vector = jsonData["rotationAngle"];
				std::stringstream vectorSS(vector);
				vectorSS >> rotationAngle[0] >> rotationAngle[1] >> rotationAngle[2];
			}
			if (jsonData.contains("translation")) {
				std::string vector = jsonData["translation"];
				std::stringstream vectorSS(vector);
				vectorSS >> translation[0] >> translation[1] >> translation[2];
			}
			if (jsonData.contains("scale")) {
				std::string vector = jsonData["scale"];
				std::stringstream vectorSS(vector);
				vectorSS >> scale[0] >> scale[1] >> scale[2];
			}

			transformModel = glm::translate(translation) * glm::rotate(rotation, rotationAngle) * glm::scale(scale);

			for (auto anim : jsonData["clips"]) {
				BMWAnimSegment* clip = new BMWAnimSegment;
				clip->anim = animCountOffset;
				if (anim.contains("file")) {
					clip->start = 0;

					std::string filename = anim["file"];
					BMWModeLData* fileAnim = loadFile(ResourceLoader::Instance().getModel(filename));
					animlist.push_back(fileAnim->animlist[0]);
					
					clip->end = fileAnim->animlist[0]->duration;
					animCountOffset++;
					subfilesLoaded.push_back(ResourceLoader::Instance().getModel(filename));
				}
				else {
					clip->start = anim["start"];
					clip->end = anim["end"];
				}

				if (anim.contains("loop"))
					clip->isLoop = anim["loop"];

				if (anim.contains("hold"))
					clip->isHold = anim["hold"];

				std::string clipName = anim["name"];

				(animations)[clipName] = clip;
			}

			for (auto tex : jsonData["textureOverrides"]) {
				texOverrides.push_back(tex["texture"]);
			}
		}

		// handle file header
		readString(&in);

		// handle texture list
		unsigned int materials = readInt(&in);
		for (int i = 0; i < materials; i++) {
			unsigned int textures = readInt(&in);
			for (int j = 0; j < textures; j++) {
				(texlist).push_back(readString(&in));
			}
		}

		// handle texture overrides from json
		for (int i = 0; i < texlist.size() && i < texOverrides.size(); i++)
			texlist[i] = texOverrides[i];
		for (int i = texlist.size(); i < texOverrides.size(); i++)
			texlist.push_back(texOverrides[i]);

		// handle anims
		unsigned int anims = readInt(&in);
		for (int i = 0; i < anims; i++) {
			unsigned int duration = readInt(&in);
			unsigned int speed = readInt(&in);
			unsigned int bones = readInt(&in);

			std::map<int, std::vector<glm::mat4>> transMap;
			for (int j = 0; j < bones; j++) {
				unsigned int boneAffected = readInt(&in);
				std::vector<glm::mat4> transforms;
				for (int k = 0; k <= duration; k++) {
					transforms.push_back(readTransform(&in));
				}
				transMap[boneAffected] = transforms;
			}

			std::vector<std::map<int, glm::mat4>> trans((transMap.begin()->second).size());
			for (auto it : transMap) {
				for (int j = 0; j < it.second.size(); j++)
					trans[j][it.first] = it.second[j];
			}

			BMWAnim* current = new BMWAnim();
			current->duration = duration;
			current->rate = speed;
			current->transforms = trans;
			animlist.push_back(current);
		}

		// handle meshes
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

		// handle nodes
		BMWNode* root = readNode(&in, &nodeIDs);

		// handle bones
		int boneNameNum = readInt(&in);
		for (int i = 0; i < boneNameNum; i++)
			boneNames.push_back({ readInt(&in), readString(&in) });

		// handle animations by name
		animCountOffset = 0;
		if (jsonIn) {
			for (auto anim : jsonData["clips"]) {
				if (anim.contains("file")) {

					std::map<int, std::string> parentIDtoName;
					std::map<int, std::string> childIDtoName;
					std::map<std::string, int> parentNametoID;
					std::map<std::string, int> childNametoID;

					for (int i = 0; i < boneNames.size(); i++) {
						parentIDtoName[boneNames[i].first] = boneNames[i].second;
						parentNametoID[boneNames[i].second] = boneNames[i].first;
					}

					BMWModeLData* fileAnim = loadFile(subfilesLoaded[animCountOffset]);
					for (int i = 0; i < fileAnim->boneNames.size(); i++) {
						childIDtoName[fileAnim->boneNames[i].first] = fileAnim->boneNames[i].second;
						childNametoID[fileAnim->boneNames[i].second] = fileAnim->boneNames[i].first;
					}

					for (int i = 0; i < animlist[animCountOffset]->transforms.size(); i++) {
						std::map<int, glm::mat4> newmap;
						for (auto it : animlist[animCountOffset]->transforms[i]) {
							if (childIDtoName.count(it.first) && parentNametoID.count(childIDtoName[it.first])) {
								newmap[parentNametoID[childIDtoName[it.first]]] = it.second;
							}
							else {
								newmap[it.first] = it.second;
							}
						}
						animlist[animCountOffset]->transforms[i] = newmap;
					}
					
					animCountOffset++;
				}
			}
		}

		// cache results
		m_stored[file].texlist = texlist;
		m_stored[file].meshlist = meshlist;
		m_stored[file].indexlist = indexlist;
		m_stored[file].root = root;
		m_stored[file].nodeIDs = nodeIDs;
		m_stored[file].boneWeights = boneWeights;
		m_stored[file].animlist = animlist;
		m_stored[file].animations = animations;
		m_stored[file].defaultAnim = defaultAnim;
		m_stored[file].transform = transformModel;
		m_stored[file].boneNames = boneNames;

		return &m_stored[file];
	}

	std::string BMWLoader::readString(std::ifstream* in) {
		std::string toret = "";
		char next;

		// read until null
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

		BMWNode* toret = new BMWNode();
		toret->transform = transform;
		toret->meshNum = meshNum;
		toret->meshIDs = meshes;
		toret->children = children;

		(*nodeIDs).insert({ nodeId, toret });
		return toret;
	}
}