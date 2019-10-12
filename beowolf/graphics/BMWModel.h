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
		void update(float delta);
		void render(glm::mat4 view, glm::mat4 proj);

	private:
		std::vector<std::string> m_textures;
		std::vector<std::vector<Vertex>> m_meshes;
		BMWNode m_rootNode;
	};
}