#include <W_Common.h>
#include <BMWLoader.h>
#include <vector>
#include <string>
#include <utility>
#include <W_Material.h>
#include <W_VertexBuffer.h>
#include <W_VertexDeclaration.h>
#include <W_IndexBuffer.h>
#include <W_Program.h>

namespace wolf
{
	class BMWModel
	{
	public:
		BMWModel(std::string file, std::string vertexShader, std::string pixelShader);
		void update(float delta);
		void render(glm::mat4 view, glm::mat4 proj, bool renderAlphas);

		glm::mat4 getTransform();
		void setTransform(glm::mat4 transform);

	private:
		struct Mesh
		{
			unsigned int size;
			bool isTransparent = false;
			wolf::VertexBuffer* m_pVB;
			wolf::IndexBuffer* m_pIB;
			wolf::VertexDeclaration* m_pDecl;
			wolf::Program* m_pProg;
			wolf::Texture* m_pTex;
		};

		struct NodeMesh
		{
			glm::mat4 transform;
			int meshID;
		};

		void renderMesh(glm::mat4 world, glm::mat4 view, glm::mat4 proj, unsigned int meshID);

		std::vector<std::string> m_textures;
		std::vector<std::vector<Vertex>> m_vertices;
		std::vector<std::vector<unsigned int>> m_indices;
		std::vector<Mesh> m_meshes;
		std::vector<Material*> m_materials;
		glm::mat4 transform;
		BMWNode m_rootNode;
		std::vector<NodeMesh> m_toRender;
	};
}