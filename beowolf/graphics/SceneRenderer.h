#ifndef SceneRendererRENDERER_H
#define SceneRendererRENDERER_H

#include "W_Common.h"
#include "Node.h"
#include <vector>
#include "QuadTree.h"

namespace wolf
{
	class SceneRenderer
	{
	public:
		static SceneRenderer& getInstance()
		{
			static SceneRenderer instance;
			return instance;
		}

		void AddNode(Node* node);
		void Update(float delta, glm::mat4 cull);
		void Render(glm::mat4 projview);
		void GenerateQuadtree();
		QuadTree* GetQuadtree();

	private:
		SceneRenderer() {}

		std::vector<Node*> m_nodes;
		std::vector<Node*> m_toRender;
		QuadTree* m_quadtree;

	public:
		SceneRenderer(SceneRenderer const&) = delete;
		void operator=(SceneRenderer const&) = delete;
	};
}

#endif