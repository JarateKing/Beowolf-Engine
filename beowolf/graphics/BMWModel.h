#ifndef BMWMODEL_H
#define BMWMODEL_H

#include <W_Common.h>
#include <BMWLoader.h>
#include <vector>
#include <map>
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
		void render(glm::mat4 view, glm::mat4 proj, RenderFilterType type);

		glm::mat4 getTransform();
		void setModelColor(glm::vec3 color);
		void setModelAdditive(glm::vec3 color);
		void setModelFilter(glm::vec3 color);
		void setTransform(glm::mat4 transform);
		void setAnim(std::string name);
		bool getIsAnimationRunning();

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

		std::vector<std::string>* m_textures;
		std::vector<std::vector<Vertex>>* m_vertices;
		std::vector<std::vector<unsigned int>>* m_indices;
		std::vector<Mesh> m_meshes;
		std::vector<Material*>* m_materials;
		std::map<int, BMWNode*>* m_nodeIDs;
		std::map<int, std::vector<std::pair<int, float>>>* m_boneWeights;
		std::vector<BMWAnim*>* m_anims;
		std::map<std::string, BMWAnimSegment*>* m_animFrames;
		glm::mat4 transform;
		glm::mat4 m_transformModel;
		BMWAnimSegment* m_currentAnimation;
		std::string* m_defaultAnimation;
		glm::mat4 m_boneMatrix[128];
		bool m_hasAnimations;
		float m_animationFrame;
		BMWNode* m_rootNode;
		std::vector<NodeMesh> m_toRender;
		int m_currentAnimNum = 0;
		bool m_isAnimationDone = false;

		glm::vec3 m_modelColor = glm::vec3(1, 1, 1);
		glm::vec3 m_modelAdditive = glm::vec3(0, 0, 0);
		glm::vec3 m_modelFilter = glm::vec3(1, 1, 1);
	};
}
#endif