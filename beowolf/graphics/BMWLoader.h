#include <W_Common.h>
#include <vector>
#include <map>
#include <fstream>
#include <string>

namespace wolf
{
	struct BMWNode {
		glm::mat4 transform;
		unsigned int meshNum;
		unsigned int* meshIDs;
		std::vector<BMWNode> children;
	};

	class BMWLoader
	{
	public:
		static BMWLoader& getInstance()
		{
			static BMWLoader instance;
			return instance;
		}

		void BMWLoader::loadFile(std::string file, std::vector<std::string>* texlist, std::vector<std::vector<Vertex>>* meshlist, std::vector<std::vector<unsigned int>>* indexlist, BMWNode* root, std::map<int, BMWNode*>* nodeIDs, std::map<int, std::vector<std::pair<int, float>>>* boneWeights);

	private:
		BMWLoader() {}
		std::string readString(std::ifstream* in);
		unsigned int readInt(std::ifstream* in);
		float readFloat(std::ifstream* in);
		glm::mat4 readTransform(std::ifstream* in);
		BMWNode readNode(std::ifstream* in, std::map<int, BMWNode*>* nodeIDs);

	public:
		BMWLoader(BMWLoader const&) = delete;
		void operator=(BMWLoader const&) = delete;
	};
}