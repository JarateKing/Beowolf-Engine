#ifndef QUADTREE_H
#define QUADTREE_H

#include "W_Common.h"
#include "Node.h"
#include <vector>
#include "W_BufferManager.h"
#include "W_ProgramManager.h"
#include "W_VertexDeclaration.h"

namespace wolf
{
	struct QuadTreeNode
	{
		Node* node;
		long lastRendered;

		QuadTreeNode(Node* node)
		{
			this->node = node;
			lastRendered = 0;
		}
	};

	class QuadTree
	{
	public:
		QuadTree(float xpos, float zpos, float width, float depth);
		~QuadTree();
		bool AddNode(QuadTreeNode* child);

		bool IsSphereInside(glm::vec3 point, float radius);
		bool isSphereEngulfing(glm::vec3 point, float radius);
		bool QuadTree::IsFrustumInside(Plane* frustum);
		std::vector<Node*> GetVisible(long currentRender, Plane* cull);

		void DebugRender(glm::mat4 projview);

	private:
		std::vector<QuadTree*> m_children;
		std::vector<QuadTreeNode*> m_bigNodes;
		std::vector<QuadTreeNode*> m_nodes;

		float m_xpos;
		float m_zpos;
		float m_width;
		float m_depth;

		Program*  m_Program;
		VertexBuffer* m_VB;
		VertexDeclaration* m_Decl;
	};
}

#endif