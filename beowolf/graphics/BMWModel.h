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
		BMWModel(const std::string& file, const std::string& vertexShader, const std::string& pixelShader, const std::string& shadowVertexShader, const std::string& shadowPixelShader);
		~BMWModel();
		void update(float delta);
		void render(glm::mat4 view, glm::mat4 proj, glm::mat4 lightSpaceMatrix, RenderFilterType type, bool shadowPass, unsigned int depthMapTexture, bool instanced, float minheight = -10000, float maxheight = 100000);

		glm::mat4 getTransform();
		void setModelColor(const glm::vec3& color);
		void setModelAdditive(const glm::vec3&  color);
		void setModelFilter(const glm::vec3&  color);
		void setTransform(const glm::mat4& transform);
		void setAnim(const std::string& name);
		void setLightAmbient(const glm::vec4& light);
		void setLightDiffuse(const glm::vec4& light);
		void setLightSpecular(const glm::vec4& light);
		void setLightDir(const glm::vec3& direction);
		void setViewDir(const glm::vec3& direction);
		void setInstancedVariable(const std::vector<glm::mat4>& instancedV);
		bool getIsAnimationRunning();
		BMWAnimSegment* getAnim();
		std::string getAnimName();
		bool isAnimDefault();
		bool canAnimate();

	private:
		struct Mesh
		{
			unsigned int size;
			bool isTransparent = false;
			wolf::VertexBuffer* m_pVB;
			wolf::IndexBuffer* m_pIB;
			wolf::VertexDeclaration* m_pDecl;
			wolf::Program* m_pProg;
			wolf::Program* m_pShadowProg;
			wolf::Texture* m_pTex;
		};

		struct NodeMesh
		{
			glm::mat4 transform;
			int meshID;
		};

		void renderMesh(glm::mat4 world, glm::mat4 view, glm::mat4 proj, glm::mat4 lightSpaceMatrix, unsigned int meshID, bool shadowPass, unsigned int depthMapTexture, bool instanced, float minheight, float maxheight);

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
		glm::mat4 m_instancedWorld[64];
		bool m_hasAnimations;
		float m_animationFrame;
		BMWNode* m_rootNode;
		std::vector<NodeMesh> m_toRender;
		std::vector<glm::mat4> instanceInfo;
		int m_currentAnimNum = 0;
		bool m_isAnimationDone = false;

		glm::vec3 m_modelColor = glm::vec3(1, 1, 1);
		glm::vec3 m_modelAdditive = glm::vec3(0, 0, 0);
		glm::vec3 m_modelFilter = glm::vec3(1, 1, 1);

		glm::vec4 m_lightAmbient = glm::vec4(1, 1, 1, 1);
		glm::vec4 m_lightDiffuse = glm::vec4(1, 1, 1, 1);
		glm::vec4 m_lightSpecular = glm::vec4(1, 1, 1, 1);
		glm::vec3 m_lightDir = glm::vec3(1, 1, 1);
		glm::vec3 m_viewDir = glm::vec3(1, 1, 1);
	};
}
#endif