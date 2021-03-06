#ifndef BMWLOADER_H
#define BMWLOADER_H

#include <W_Common.h>
#include <vector>
#include <map>
#include <unordered_map>
#include <fstream>
#include <string>
#include <iostream>

namespace wolf
{
	// the tree structure for nodes (including bones)
	struct BMWNode {
		glm::mat4 transform;
		unsigned int meshNum;
		unsigned int* meshIDs;
		std::vector<BMWNode*> children;
	};

	// the set of animation transforms
	struct BMWAnim {
		unsigned int duration;
		unsigned int rate;
		std::vector<std::map<int, glm::mat4>> transforms;
		BMWAnim() {};
	};

	// the metadata for each animation clip
	struct BMWAnimSegment {
		unsigned int anim;
		unsigned int start;
		unsigned int end;
		bool isLoop = false;
		bool isHold = false;
	};

	// the entire loaded data from a bmw file
	struct BMWModeLData {
		std::vector<std::string> texlist;
		std::vector<std::vector<Vertex>> meshlist;
		std::vector<std::vector<unsigned int>> indexlist;
		BMWNode* root;
		std::map<int, BMWNode*> nodeIDs;
		std::map<int, std::vector<std::pair<int, float>>> boneWeights;
		std::vector<BMWAnim*> animlist;
		std::map<std::string, BMWAnimSegment*> animations;
		std::string defaultAnim;
		glm::mat4 transform;
		std::vector<std::pair<int, std::string>> boneNames;
	};

	class BMWLoader
	{
	public:
		static BMWLoader& getInstance()
		{
			static BMWLoader instance;
			return instance;
		}

		BMWModeLData* loadFile(const std::string& file);

	private:
		BMWLoader() {}
		std::string readString(std::ifstream* in);
		unsigned int readInt(std::ifstream* in);
		float readFloat(std::ifstream* in);
		glm::mat4 readTransform(std::ifstream* in);
		BMWNode* readNode(std::ifstream* in, std::map<int, BMWNode*>* nodeIDs);

		std::map<std::string, BMWModeLData> m_stored;

	public:
		BMWLoader(BMWLoader const&) = delete;
		void operator=(BMWLoader const&) = delete;
	};
}
#endif