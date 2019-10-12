#include <W_Common.h>
#include <BMWLoader.h>
#include <vector>
#include <string>

namespace wolf
{
	class BMWModel
	{
	public:
		BMWModel(std::string file, std::string vertexShader, std::string pixelShader);

	private:
		std::vector<std::string> m_textures;
		std::vector<std::vector<Vertex>> m_meshes;
		BMWNode m_rootNode;
	};
}